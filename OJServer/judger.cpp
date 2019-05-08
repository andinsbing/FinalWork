#include "judger.h"
#include "../library/file/filesystem.h"
#include "../library/global_config/globalconfig.h"
#include "../library/json/jsonfunction.h"
#include <QProcess>

Judger::Judger(QObject* parent) : QObject(parent), process(new QProcess(this))
{
    Q_ASSERT(JsonFunction::hasKey(Global::config, "compiler_path"));
    compilePath = Global::config["compiler_path"].toString();
    programPath = compilePath + R"(\Bin\Judger.exe)";
    Q_ASSERT(FileSystem().hasFile(programPath));
}

QJsonObject Judger::judge(const QJsonObject& task)
{
    Q_ASSERT(JsonFunction::hasKey(
        task, { "time_limit", "memory_limit", "language", "index", "task_id", "code" }));

    QString codePath;
    QString language = task["language"].toString();
    if (language == "CPP") {
        codePath = compilePath + R"(\Code\Cpp\a.cpp)";
    } else if (language == "JAVA") {
        codePath = compilePath + R"(\Code\JAVA\A.java)";
    } else if (language == "C") {
        codePath = compilePath + R"(\Code\C\a.c)";
    } else {
        Q_UNREACHABLE();
    }
    FileSystem().makeFile(codePath, task["code"].toString().toUtf8());

    QStringList argument;
    argument << "-t" << QString::number(task["time_limit"].toInt()) << "-m"
             << QString::number(task["memory_limit"].toInt()) << "-c" << language << "-i"
             << QString::number(task["index"].toInt());
    process->setWorkingDirectory(compilePath);
    process->start(programPath, argument);

    QJsonObject result;
    JsonFunction::copy(result, task, "task_id");

    if (!process->waitForFinished(task["time_limit"].toInt() + 60 * 1000)) {
        process->kill();  // has cost too much time, may be some errrors inner occured
        result.insert("result", "JudgerInnerErrror");
        result.insert("time", 0);
        result.insert("memory", 0);
    } else {
        process->kill();  // windows bug, ensure the process exit in time
        auto&& ouput      = QString::fromLocal8Bit(process->readAllStandardOutput());
        auto&& outputList = ouput.split(',');
        if (outputList.size() != 3) {
            Q_ASSERT_X(false, "judger_output", ouput.toStdString().c_str());
        }
        result.insert("result", outputList[0]);
        result.insert("time", outputList[1]);
        result.insert("memory", outputList[2]);
    }
    return result;
}
