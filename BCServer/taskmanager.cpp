#include "taskmanager.h"
#include "../library/json/jsonfunction.h"
#include "../library/network/networkidfunction.h"
#include <QTime>
#include <algorithm>

TaskManager::TaskManager(QObject* parent) : QObject(parent) {}

void TaskManager::addTask(const QJsonObject& json)
{
    QJsonObject task;
    JsonFunction::copy(task, json, { "code", "index", "language", "user_id" });
    task.insert("task_id", taskList.size());
    task.insert("time_limit", 1000);
    task.insert("memory_limit", 100);
    task.insert("start_time", QTime::currentTime().toString());
    taskQueue.append(taskList.size());
    taskList.append(task);
}

QJsonObject TaskManager::takeTask()
{
    if (taskQueue.empty()) {
        return { { "error", "task not ready" } };  // this task will be discarded in oj server
    }
    auto id = taskQueue.takeFirst();
    taskStarted.append(id);
    return taskList.at(id);
}

void TaskManager::finishTask(const QJsonObject& json)
{
    Q_ASSERT(JsonFunction::hasKey(json, "task_id"));
    auto id = json["task_id"].toInt();
    Q_ASSERT(taskStarted.contains(id));
    taskStarted.removeOne(id);

    JsonFunction::copy(taskList[id], json, { "time", "memory", "result" });
    taskFinished.append(id);
    emit newTaskFinished(taskList[id]);
}

QJsonArray TaskManager::getTaskFinished()
{
    QJsonArray array;
    for (auto& i : taskFinished) {
        array.append(taskList[i]);
    }
    return array;
}

bool TaskManager::hasTask() const
{
    return !taskQueue.isEmpty();
}
