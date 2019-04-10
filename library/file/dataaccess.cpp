#include "dataaccess.h"
#include "../json/jsonfunction.h"
#include "filesystem.h"
#include <QDir>

DataAccess::DataAccess(QObject* parent) : QObject(parent), fileSystem(new FileSystem(this)) {}

QJsonObject DataAccess::importTopic(const QString& dir)
{
    Q_ASSERT(fileSystem->hasDIr(dir));
    auto sampleDir = dir + "\\Sample";
    auto testDir   = dir + "\\Test";

    Q_ASSERT(fileSystem->hasDIr({ sampleDir, testDir }));

    auto descriptionPath = dir + "\\description.html";
    auto profilePath     = dir + "\\profile.json";

    Q_ASSERT(fileSystem->hasFile({ descriptionPath, profilePath }));

    auto profile = fileSystem->readJson(profilePath);
    Q_ASSERT(JsonFunction::hasKey(profile, { "title", "time_limit", "memory_limit" }));

    QJsonObject json;
    json.insert("description", QString::fromUtf8(fileSystem->readFile(descriptionPath)));
    json.insert("title", profile["title"]);
    json.insert("time_limit", profile["time_limit"]);
    json.insert("memory_limit", profile["memory_limit"]);
    auto sample = readSequenceFile(sampleDir);
    auto test   = readSequenceFile(testDir);
    Q_ASSERT(!sample.empty() && !test.empty());
    json.insert("sample", sample);
    json.insert("test", test);
    return json;
}

void DataAccess::exportTopicTest(const QJsonArray& array, const QString& path)
{
    Q_ASSERT(array.size() > 0);

    for (const auto& i : array) {
        const auto& obj = i.toObject();
        Q_ASSERT(JsonFunction::hasKey(obj, { "index", "test" }));
        auto        index  = obj["index"].toInt();
        auto        floder = path + QString("\\Topic%1").arg(index);
        const auto& test   = obj["test"].toArray();
        Q_ASSERT(test.size() > 0);
        for (int j = 0; j < test.size(); j++) {
            const auto& testObj = test[j].toObject();
            fileSystem->makeFile(floder + QString("\\%1.in").arg(j + 1),
                                 testObj["input"].toString().toUtf8());
            fileSystem->makeFile(floder + QString("\\%1.out").arg(j + 1),
                                 testObj["output"].toString().toUtf8());
        }
    }
}

QJsonArray DataAccess::readSequenceFile(const QString& dir)
{
    QJsonArray jsonArray;
    QString    inputPath;
    QString    outputPath;

    for (int i = 1;; i++) {
        inputPath  = dir + QString("\\%1.in").arg(i);
        outputPath = dir + QString("\\%1.out").arg(i);
        if (!fileSystem->hasFile({ inputPath, outputPath })) {
            break;
        }
        QJsonObject json;
        json.insert("input", QString::fromUtf8(fileSystem->readFile(inputPath)));
        json.insert("output", QString::fromUtf8(fileSystem->readFile(outputPath)));
        jsonArray.push_back(json);
    }
    return jsonArray;
}
