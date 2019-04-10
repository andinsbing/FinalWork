#include "bcservernetworkmodel.h"
#include "../library/file/filesystem.h"
#include "../library/global_config/globalconfig.h"
#include "../library/json/jsonfunction.h"
#include "../library/network/networkidfunction.h"
#include "taskmanager.h"
#include <QJsonArray>
#include <QTime>

BCServerNetworkModel::BCServerNetworkModel(QObject* parent)
    : NetworkModel(parent), taskManager(new TaskManager(this))
{
    connect(taskManager, &TaskManager::newTaskFinished, [this](const QJsonObject& data) {
        QJsonObject json;
        json.insert("action", "push");
        json.insert("type", "new_finished_task");
        json.insert("data", data);
        emit this->broadcastToServer(json);
    });
}

void BCServerNetworkModel::updateTopic(const QJsonObject& json)
{
    this->topic = json;
}

void BCServerNetworkModel::updateUser(const QJsonObject& json)
{
    this->user = json;
}

void BCServerNetworkModel::updateContest(const QJsonObject& json)
{
    this->contest = json;
}

void BCServerNetworkModel::newPullEvent(const QJsonObject& json)
{
    Q_ASSERT(isServerJson(json) || isOJServerJson(json));
    QJsonObject reply;
    reply.insert("action", "push");
    const auto& type = json["type"].toString();
    reply.insert("type", type);
    NetworkIDFunction::copyID(reply, json);
    if (isServerJson(json)) {
        if (type == "topic") {
            reply.insert("data", topic);
        } else if (type == "user") {
            reply.insert("data", user);
        } else if (type == "finished_task") {
            reply.insert("data", taskManager->getTaskFinished());
        } else {
            Q_UNREACHABLE();
        }
    } else if (isOJServerJson(json)) {
        if (type == "topic") {
            reply.insert("data", topic);
        } else {
            Q_UNREACHABLE();
        }
    }
    emit sendJson(reply);
}

void BCServerNetworkModel::newPushEvent(const QJsonObject& json)
{
    Q_ASSERT(isOJServerJson(json));
    auto&& type = json["type"].toString();
    if (type == "task_result") {
        auto result = json;
        NetworkIDFunction::removeID(result);
        taskManager->finishTask(result);
        OJReadyLIst.push_back(NetworkIDFunction::getID(json));
        dispatchTask();
        OJTask[NetworkIDFunction::getID(json)] = QJsonObject{};
    } else if (type == "ready") {
        OJReadyLIst.push_back(NetworkIDFunction::getID(json));
        dispatchTask();
    } else {
        Q_UNREACHABLE();
    }
}

void BCServerNetworkModel::newSubmitEvent(const QJsonObject& json)
{
    Q_ASSERT(isServerJson(json));
    taskManager->addTask(json);
    dispatchTask();
}

void BCServerNetworkModel::start()
{
    addTestSubmit();
    emit waitForContestData();
}

void BCServerNetworkModel::networkDisconnected(qint32 networkID)
{
    auto task = OJTask[networkID];
    if (!task.isEmpty()) {
        taskManager->addTask(task);
        OJTask[networkID] = QJsonObject{};
    }
    if (OJReadyLIst.contains(networkID)) {
        OJReadyLIst.removeOne(networkID);
    }
}

void BCServerNetworkModel::addTestSubmit()
{
    QJsonObject task;
    task.insert("user_id", 1);
    task.insert("language", "CPP");
    task.insert("time_limit", 1000);
    task.insert("memory_limit", 100);
    task.insert("index", 1);
    Q_ASSERT(JsonFunction::hasKey(Global::config, "test_cpp_code"));
    task.insert("code", QString::fromUtf8(
                            FileSystem().readFile(Global::config["test_cpp_code"].toString())));
    taskManager->addTask(task);
    taskManager->addTask(task);
    taskManager->addTask(task);
}

void BCServerNetworkModel::dispatchTask()
{
    if (OJReadyLIst.isEmpty() || !taskManager->hasTask()) {
        return;
    }

    QJsonObject json;
    json.insert("action", "push");
    json.insert("type", "task");
    NetworkIDFunction::setID(json, -1);

    while (!OJReadyLIst.isEmpty() && taskManager->hasTask()) {
        auto task = taskManager->takeTask();
        json.insert("data", task);
        auto ID = OJReadyLIst.first();
        NetworkIDFunction::replaceID(json, OJReadyLIst.takeFirst());
        emit sendJson(json);
        OJTask[ID] = task;
    }
}
