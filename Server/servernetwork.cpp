#include "servernetwork.h"
#include "../library/global_config/globalconfig.h"

ServerNetwork::ServerNetwork(QObject* parent) : Network(parent) {}

void ServerNetwork::sendJsonToBCServer(QJsonObject json)
{
    Q_ASSERT(!hasID(json));
    markSpecificID(json, BCServerNetworkID);
    this->sendJson(json);
}

void ServerNetwork::broadcastToClient(QJsonObject json)
{
    Q_ASSERT(!hasID(json));
    for (auto& ID : this->clientIDSet) {
        if (isValidNetwork(ID)) {
            markSpecificID(json, ID);
            sendJson(json);
            unmarkID(json);
        }
    }
}

void ServerNetwork::start()
{
    listen();
    connectToBCServer();
}

bool ServerNetwork::PreprocessReceivedJson(QJsonObject& json)
{
    const auto& action = json["action"].toString();
    if (action == "network_config") {
        handleNetworkConfigAction(json);
        return true;
    }
    bool handled = false;
    auto ID      = getID(json);
    if (clientIDSet.contains(ID)) {
        json.insert("role", Global::clientRole);
    } else if (ID == BCServerNetworkID) {
        json.insert("role", Global::BCServerRole);
    } else {
        Q_UNREACHABLE();
    }
    return handled;
}

void ServerNetwork::listen()
{
    const auto& role   = Global::serverRole;
    const auto& config = Global::config;
    Q_ASSERT(config.contains(role));
    Q_ASSERT(config[role].toObject().contains("port"));
    auto ip = config[role].toObject()["port"].toInt();
    listenTo(static_cast<quint16>(ip));
}

void ServerNetwork::connectToBCServer()
{
    const auto& role   = Global::BCServerRole;
    const auto& config = Global::config;
    Q_ASSERT(config.contains(role));
    auto serverConfig = config[role];
    Q_ASSERT(serverConfig.isObject());
    auto serverConfigObj = serverConfig.toObject();
    Q_ASSERT(serverConfigObj.contains("ip"));
    Q_ASSERT(serverConfigObj.contains("port"));
    auto ip           = serverConfigObj["ip"].toString();
    auto port         = serverConfigObj["port"].toInt();
    BCServerNetworkID = connectToHost(ip, static_cast<quint16>(port));
    QJsonObject handshakeInfo;
    handshakeInfo["action"] = "network_config";
    handshakeInfo["type"]   = "handshake";
    handshakeInfo["role"]   = Global::Local::role;
    sendJsonToBCServer(handshakeInfo);
}

void ServerNetwork::handleNetworkConfigAction(const QJsonObject& json)
{
    Q_ASSERT(json["action"] == "network_config");
    const auto& type = json["type"].toString();
    if (type == "handshake") {
        auto        ID   = getID(json);
        const auto& role = json["role"].toString();
        if (role == Global::clientRole) {
            Q_ASSERT(!clientIDSet.contains(ID));
            this->clientIDSet.insert(ID);
        } else if (role == Global::BCServerRole) {
            Q_ASSERT(!backEndIDSet.contains(ID));
            backEndIDSet.insert(ID);
        } else {
            Q_UNREACHABLE();
        }
    } else {
        Q_UNREACHABLE();
    }
}
