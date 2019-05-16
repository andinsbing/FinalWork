#ifndef DOCKERJUDGER_H
#define DOCKERJUDGER_H

#include <QJsonArray>
#include <QJsonObject>
#include <QObject>

class DockerJudger : public QObject
{
    Q_OBJECT

private:
    struct JudgeResult
    {
        QString type;
        qint32  time;
        qint32  memory;
    };

public:
    explicit DockerJudger(QObject* parent = nullptr);
    QJsonObject judge(const QJsonObject& task);

public slots:
    void updateTopic(const QJsonObject& json);

private:
    class QProcess* process;
    QString         compilePath;
    QString         dockerPath;
    QJsonArray      topicArray;
};

#endif  // DOCKERJUDGER_H
