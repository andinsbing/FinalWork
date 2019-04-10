#ifndef DATAACCESS_H
#define DATAACCESS_H

#include <QJsonArray>
#include <QJsonArray>
#include <QJsonObject>
#include <QObject>

class DataAccess : public QObject
{
    Q_OBJECT
public:
    explicit DataAccess(QObject* parent = nullptr);

public:
    QJsonObject importTopic(const QString& dir);
    void        exportTopicTest(const QJsonArray& array, const QString& path);

private:
    QJsonArray readSequenceFile(const QString& dir);

private:
    class FileSystem* fileSystem;
};

#endif  // DATAACCESS_H
