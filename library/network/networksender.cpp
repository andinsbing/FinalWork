#include "networksender.h"
#include "network.h"
NetworkSender::NetworkSender(QObject* parent) : QObject(parent) {}

void NetworkSender::sendRequestActionJson(QJsonObject json)
{
    Q_ASSERT(!json.contains("action"));
    Q_ASSERT(json.contains("type"));
    json.insert("action", "request");
    emit sendJson(json);
}

void NetworkSender::sendPullActionJson(QJsonObject json)
{
    Q_ASSERT(!json.contains("action"));
    Q_ASSERT(json.contains("type"));
    json.insert("action", "pull");
    emit sendJson(json);
}

void NetworkSender::sendPushActionJson(QJsonObject json)
{
    Q_ASSERT(!json.contains("action"));
    Q_ASSERT(json.contains("type"));
    json.insert("action", "push");
    emit sendJson(json);
}

void NetworkSender::sendReplyActionJson(QJsonObject json)
{
    Q_ASSERT(!json.contains("action"));
    Q_ASSERT(json.contains("type"));
    json.insert("action", "reply");
    emit sendJson(json);
}

void NetworkSender::sendCammandActionJson(QJsonObject json)
{
    Q_ASSERT(!json.contains("action"));
    Q_ASSERT(json.contains("type"));
    json.insert("action", "command");
    emit sendJson(json);
}

void NetworkSender::setNetwork(Network* network)
{
    connect(this, &NetworkSender::sendJson, network, &Network::sendJson);
}
