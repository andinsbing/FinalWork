#include "ojservernetwork.h"
#include "../library/global_config/globalconfig.h"

OJServerNetwork::OJServerNetwork(QObject* parent) : Network(parent) {}

void OJServerNetwork::start()
{
    connectToBCServer();
}

void OJServerNetwork::sendJsonToBCServer(QJsonObject json)
{
    Q_ASSERT(!hasID(json));
    markSpecificID(json, BCServerNetworkID);
    this->sendJson(json);
}

bool OJServerNetwork::PreprocessReceivedJson(QJsonObject& json)
{
    Q_UNUSED(json);
    return false;
}

void OJServerNetwork::connectToBCServer()
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
