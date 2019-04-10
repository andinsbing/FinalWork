#include "clientmodel.h"
#include <QDebug>
#include <QEventLoop>

ClientModel::ClientModel(QObject* parent) : NetworkModel(parent) {}

void ClientModel::start()
{
    QJsonObject json;
    json.insert("action", "pull");
    json.insert("type", "topic");
    emit sendJson(json);
    json["type"] = "finished_task";
    emit sendJson(json);
    emit waitForLogin();
}

void ClientModel::login(const QString& name, const QString& password)
{
    QJsonObject json;
    json.insert("action", "request");
    json.insert("type", "login");
    json.insert("name", name);
    json.insert("password", password);
    emit sendJson(json);
}

void ClientModel::submit(int index, const QString& code, const QString& language)
{
    QJsonObject json;
    json.insert("action", "request");
    json.insert("type", "submit");
    json.insert("code", code);
    json.insert("language", language);
    json.insert("index", index);
    emit sendJson(json);
}

void ClientModel::info(const QString& text)
{
    QJsonObject json;
    json.insert("action", "command");
    json.insert("type", "info");
    json.insert("text", text);
    emit sendJson(json);
}

void ClientModel::newLoginEvent(const QJsonObject& json) {}

void ClientModel::newInitEvent(const QJsonObject& json)
{
    Q_ASSERT(json.contains("topic"));
    Q_ASSERT(json.contains("rank"));
    this->topic = json["topic"].toObject();
    this->rank  = json["rank"].toObject();
    emit waitForLogin();
}

void ClientModel::newInfoEvent(const QJsonObject& json) {}

void ClientModel::newPushEvent(const QJsonObject& json)
{
    const auto& type = json["type"].toString();
    if (type == "topic") {
        updateTopic(json);
    } else if (type == "new_finished_task") {
        auto tempJson = json["data"].toObject();
        task.append(tempJson);
        emit taskFinished(tempJson);
    } else if (type == "finished_task") {
        task = json["data"].toArray();
        for (const auto i : task) {
            emit taskFinished(i.toObject());
        }
    } else {
        Q_UNREACHABLE();
    }
}

void ClientModel::acceptLoginResult(const QJsonObject& json)
{
    const auto& result = json["result"];
    if (result == "accept") {
        emit loginResult(true);
    } else if (result == "deny") {
        emit loginResult(false, json["hint"].toString());
    } else {
        Q_UNREACHABLE();
    }
}

void ClientModel::acceptSubmitResult(const QJsonObject& json)
{
    const auto& result = json["result"];
    if (result == "accept") {
        emit submitResult(true);
    } else if (result == "deny") {
        emit submitResult(false);
    } else {
        Q_UNREACHABLE();
    }
}

void ClientModel::updateRank(const QJsonObject& json) {}

void ClientModel::updateTopic(const QJsonObject& json)
{
    Q_ASSERT(json.contains("data"));
    this->topic = json["data"].toObject();
}
