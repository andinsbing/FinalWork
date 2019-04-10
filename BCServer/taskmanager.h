#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QJsonObject>
#include <QList>
#include <QObject>

class TaskManager : public QObject
{
    Q_OBJECT
public:
    explicit TaskManager(QObject* parent = nullptr);

signals:
    void newTaskFinished(const QJsonObject& task);

public:
    void        addTask(const QJsonObject& json);
    QJsonObject takeTask();
    void        finishTask(const QJsonObject& json);
    QJsonArray  getTaskFinished();
    bool        hasTask() const;

private:
    QList<qint32>      taskQueue;
    QList<qint32>      taskStarted;
    QList<qint32>      taskFinished;
    QList<QJsonObject> taskList;
};

#endif  // TASKMANAGER_H
