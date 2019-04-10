#include "networkreceiver.h"
#include "network.h"
NetworkReceiver::NetworkReceiver(QObject* parent) : QObject(parent) {}

void NetworkReceiver::acceptJson(const QJsonObject& json)
{
    auto&& action = json["action"].toString();
    Q_ASSERT(json.contains("type"));
    auto&& type = json["type"].toString();
    if (action == "request") {
        emit newRequestAction(json, type);
    } else if (action == "command") {
        emit newCommandAction(json, type);
    } else if (action == "push") {
        emit newPushAction(json, type);
    } else if (action == "reply") {
        emit newReplyAction(json, type);
    } else if (action == "pull") {
        emit newPullAction(json, type);
    } else {
        Q_UNREACHABLE();
    }
}

void NetworkReceiver::setNetwork(Network* network)
{
    connect(network, &Network::jsonRecived, this, &NetworkReceiver::acceptJson);
}
