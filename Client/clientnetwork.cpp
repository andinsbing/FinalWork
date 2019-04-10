#include "clientnetwork.h"
#include "../library/global_config/globalconfig.h"

ClientNetwork::ClientNetwork(QObject* parent) : Network(parent) {}

void ClientNetwork::sendJsonToServer(QJsonObject json)
{
    markSpecificID(json, this->serverNetwrokID);
    this->sendJson(json);
}

void ClientNetwork::start()
{
    connetToServer();
}

bool ClientNetwork::PreprocessReceivedJson(QJsonObject& json)
{
    const auto& action = json["action"].toString();
    if (action == "network_config") {
        Q_UNREACHABLE();
        return true;
    }
    const auto& ID = getID(json);
    if (ID == serverNetwrokID) {
        json.insert("role", Global::serverRole);
    } else {
        Q_UNREACHABLE();
    }
    return false;
}

void ClientNetwork::connetToServer()
{
    const auto& role   = Global::serverRole;
    const auto& config = Global::config;
    Q_ASSERT(config.contains(role));
    auto serverConfig = config[role];
    Q_ASSERT(serverConfig.isObject());
    auto serverConfigObj = serverConfig.toObject();
    Q_ASSERT(serverConfigObj.contains("ip"));
    Q_ASSERT(serverConfigObj.contains("port"));
    auto ip         = serverConfigObj["ip"].toString();
    auto port       = serverConfigObj["port"].toInt();
    serverNetwrokID = connectToHost(ip, static_cast<quint16>(port));

    // hand shake
    QJsonObject handshakeInfo;
    handshakeInfo["action"] = "network_config";
    handshakeInfo["type"]   = "handshake";
    handshakeInfo["role"]   = Global::Local::role;
    sendJsonToServer(handshakeInfo);
}
