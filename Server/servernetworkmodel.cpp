#include "servernetworkmodel.h"
#include "../library/json/jsonfunction.h"
#include "../library/network/networkidfunction.h"
#include <QJsonArray>
#include <QMessageBox>

ServerNetworkModel::ServerNetworkModel(QObject* parent)
    : NetworkModel(parent), session(new ServerSession(this))
{
}

void ServerNetworkModel::newLoginEvent(const QJsonObject& json)
{
    Q_ASSERT(isClientJson(json));
    QJsonObject reply;
    NetworkIDFunction::copyID(reply, json);
    reply.insert("action", "reply");
    reply.insert("type", "login");
    const auto& name     = json["name"].toString();
    const auto& password = json["password"].toString();

    for (const auto i : user["array"].toArray()) {
        auto obj = i.toObject();
        Q_ASSERT(JsonFunction::hasKey(obj, { "name", "password", "id" }));
        if (obj["name"].toString() == name) {
            if (obj["password"] == password) {
                reply.insert("result", "accept");
                auto data       = session->createSession(json);
                data->name      = name;
                data->password  = password;
                data->networkID = NetworkIDFunction::getID(json);
                data->userID    = obj["id"].toInt();
                break;
            } else {
                reply.insert("result", "deny");
                reply.insert("hint", "password does not match name");
                break;
            }
        }
    }

    if (!reply.contains("result")) {
        reply.insert("result", "deny");
        reply.insert("hint", "user does not exist");
    }
    emit sendJson(reply);
}

void ServerNetworkModel::newPullEvent(const QJsonObject& json)
{
    Q_ASSERT(isClientJson(json));
    const auto& type = json["type"].toString();
    QJsonObject reply;
    NetworkIDFunction::copyID(reply, json);
    reply.insert("action", "push");
    reply.insert("type", type);
    if (type == "topic") {
        reply.insert("data", topic);
    } else if (type == "finished_task") {
        reply.insert("data", taskFinished);
    } else {
        denialOfService(json);
    }
    emit sendJson(reply);
}

void ServerNetworkModel::newSubmitEvent(const QJsonObject& json)
{
    Q_ASSERT(isClientJson(json));
    QJsonObject relay;
    relay.insert("action", "request");
    relay.insert("type", "submit");
    relay.insert("user_id", session->getSession(json)->userID);
    JsonFunction::copy(relay, json, { "code", "index", "language" });
    auto index = json["index"].toInt();
    relay.insert("time_limit", topicLimit[index].timeLimit);
    relay.insert("memory_limit", topicLimit[index].memoryLImit);
    emit sendJsonToBCServer(relay);
}

void ServerNetworkModel::newInfoEvent(const QJsonObject& json)
{
    Q_ASSERT(isClientJson(json));
    Q_ASSERT(json.contains("text"));
    QMessageBox::information(nullptr, "info", json["text"].toString());
}

void ServerNetworkModel::newPushEvent(const QJsonObject& json)
{
    Q_ASSERT(isBCServerJson(json));
    Q_ASSERT(json.contains("data"));

    const auto& type = json["type"].toString();
    if (type == "topic") {
        updateTopic(json);
    } else if (type == "user") {
        updateUser(json);
    } else if (type == "new_finished_task") {
        auto task = json["data"].toObject();
        Q_ASSERT(JsonFunction::hasKey(task, "user_id"));
        auto id = task["user_id"].toInt();
        Q_ASSERT(IDNameMap.contains(id));
        task.insert("user", IDNameMap[id]);
        this->taskFinished.append(task);
        QJsonObject tempJson;
        tempJson.insert("action", "push");
        tempJson.insert("type", "new_finished_task");
        tempJson.insert("data", task);
        emit broadcastToClient(tempJson);
    } else if (type == "finished_task") {
        auto array = json["data"].toArray();
        for (int i = 0; i < array.size(); i++) {
            auto obj = array[i].toObject();
            Q_ASSERT(JsonFunction::hasKey(obj, "user_id"));
            auto id = obj["user_id"].toInt();
            Q_ASSERT(IDNameMap.contains(id));
            obj.insert("user", IDNameMap[id]);
            this->taskFinished.append(obj);
        }
    } else {
        Q_UNREACHABLE();
    }
}
void ServerNetworkModel::start()
{
    QJsonObject json;
    json.insert("action", "pull");
    json.insert("type", "topic");
    emit sendJsonToBCServer(json);
    json["type"] = "user";
    emit sendJsonToBCServer(json);
    json["type"] = "finished_task";
    emit sendJsonToBCServer(json);
}

void ServerNetworkModel::updateTopic(const QJsonObject& topicData)
{
    this->topic       = topicData["data"].toObject();
    const auto& array = topic["array"].toArray();
    topicLimit.resize(array.size() + 1);  // start with 1
    for (int i = 1; i <= array.size(); i++) {
        const auto& obj           = array[i].toObject();
        topicLimit[i].timeLimit   = obj["time_limit"].toInt();
        topicLimit[i].memoryLImit = obj["memory_limit"].toInt();
    }
}

void ServerNetworkModel::updateUser(const QJsonObject& json)
{
    this->user = json["data"].toObject();
    for (const auto i : user["array"].toArray()) {
        auto obj = i.toObject();
        Q_ASSERT(JsonFunction::hasKey(obj, { "name", "id" }));
        IDNameMap.insert(obj["id"].toInt(), obj["name"].toString());
    }
}
