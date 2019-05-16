#include "dockerjudger.h"
#include "../library/file/filesystem.h"
#include "../library/global_config/globalconfig.h"
#include "../library/json/jsonfunction.h"
#include <QProcess>

QProcess::ProcessError lastError;

DockerJudger::DockerJudger(QObject* parent) : QObject(parent), process(new QProcess(this))
{
    Q_ASSERT(JsonFunction::hasKey(Global::config, "compiler_path"));
    compilePath = Global::config["compiler_path"].toString();
    dockerPath  = compilePath + R"(\Code\Docker)";
    Q_ASSERT(FileSystem().hasDIr(dockerPath));
    connect(process, QOverload<QProcess::ProcessError>::of(&QProcess::error),
            [](QProcess::ProcessError error) {
                for (int i = 0; i < 1; i++)
                    lastError = error;
            });
}

QJsonObject DockerJudger::judge(const QJsonObject& task)
{
    Q_ASSERT(JsonFunction::hasKey(
        task, { "time_limit", "memory_limit", "language", "index", "task_id", "code" }));

    QString codePath;
    QString language = task["language"].toString();
    if (language == "CPP") {
        codePath = dockerPath + R"(\program.cpp)";
    } else if (language == "JAVA") {
        codePath = dockerPath + R"(\Program.java)";
    } else if (language == "C") {
        codePath = dockerPath + R"(\program.c)";
    } else {
        Q_UNREACHABLE();
    }
    QString samplePath = dockerPath + R"(\sample)";
    QString inputPath  = dockerPath + R"(\std_input)";

    FileSystem().makeFile(codePath, task["code"].toString().toUtf8());

    auto               timeLimit   = task["time_limit"].toInt();
    auto               memoryLimit = task["memory_limit"].toInt();
    auto               index       = task["index"].toInt();
    const QString&     code        = task["code"].toString();
    const QJsonObject& topic       = topicArray[index - 1].toObject();
    const QJsonArray&  testArray   = topic["test"].toArray();
    Q_ASSERT(testArray.size() > 0);

    QString firstCommand =
        QString(R"(cmd /c docker run -t -v %1:/data gcctest /data/judger %2 %3 %4 %5)")
            .arg(compilePath + R"(\Code\Docker)")
            .arg(timeLimit)
            .arg(memoryLimit * 1024)
            .arg(language)
            .arg(1);  // need to compile
    QString secondCommand =
        QString(R"(cmd /c docker run  -v %1:/data gcctest /data/judger %2 %3 %4 %5)")
            .arg(compilePath + R"(\Code\Docker)")
            .arg(timeLimit)
            .arg(memoryLimit * 1024)
            .arg(language)
            .arg(0);  // doest not need to compile

    JudgeResult result;
    result.type   = "Accept";
    result.time   = 0;
    result.memory = 0;
    FileSystem fileSystem;
    for (int i = 0; i < testArray.size(); i++) {
        QString input  = testArray[i].toObject()["input"].toString();
        QString output = testArray[i].toObject()["output"].toString();

        fileSystem.makeFile(codePath, code.toUtf8());
        fileSystem.makeFile(samplePath, output.toUtf8());
        fileSystem.makeFile(inputPath, input.toUtf8());
        process->start(firstCommand);
        //        process->start("cmd /c docker");
        auto program = process->program();
        auto arg     = process->arguments();
        //        process->execute(firstCommand);
        //        process->start("dir");
        JudgeResult subResult;
        if (!process->waitForFinished(task["time_limit"].toInt() + 60 * 1000)) {
            process->error();
            process->kill();  // has cost too much time, may be some errrors inner occured
            subResult.type   = "JudgerInnerErrror";
            subResult.time   = 0;
            subResult.memory = 0;
        } else {
            process->waitForFinished();
            process->kill();  // windows bug, ensure the process exit in time
            auto&& ouput      = QString::fromLocal8Bit(process->readAllStandardOutput());
            auto&& outputList = ouput.split("\r\n");
            if (outputList.size() != 4) {
                Q_ASSERT_X(false, "judger_output", ouput.toStdString().c_str());
            }
            for (int i = 0; i < 3; i++) {
                Q_ASSERT(outputList[i].split(' ').size() == 2);
            }
            subResult.time   = outputList[0].split(' ')[1].toInt();
            subResult.memory = outputList[1].split(' ')[1].toInt();
            subResult.type   = outputList[2].split(' ')[1];
        }
        if (subResult.type != "Accept") {
            result = subResult;
            break;
        } else {
            if (subResult.time > result.time) {
                result.time = subResult.time;
            }
            if (subResult.memory > result.memory) {
                result.memory = subResult.memory;
            }
        }
    }
    QJsonObject json;
    json["result"] = result.type;
    json["time"]   = result.time;
    json["memory"] = result.memory;
    JsonFunction::copy(json, task, "task_id");
    return json;
}

void DockerJudger::updateTopic(const QJsonObject& json)
{
    Q_ASSERT(JsonFunction::hasKey(json, "array"));
    this->topicArray = json["array"].toArray();
    Q_ASSERT(topicArray.size() > 0);
}
