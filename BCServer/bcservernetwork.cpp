#include "bcservernetwork.h"
#include "../library/global_config/globalconfig.h"

BCServerNetwork::BCServerNetwork(QObject* parent) : Network(parent) {}

void BCServerNetwork::start()
{
    listen();
}

void BCServerNetwork::broadcastToServer(QJsonObject json)
{
    Q_ASSERT(!hasID(json));
    for (auto& ID : this->serverIDSet) {
        if (isValidNetwork(ID)) {
            if (isValidNetwork(ID)) {
                markSpecificID(json, ID);
                sendJson(json);
                unmarkID(json);
            }
        }
    }
}

bool BCServerNetwork::PreprocessReceivedJson(QJsonObject& json)
{
    const auto& action = json["action"].toString();
    if (action == "network_config") {
        handleNetworkConfigAction(json);
        return true;
    }
    bool handled = false;
    auto ID      = getID(json);
    if (serverIDSet.contains(ID)) {
        json.insert("role", Global::serverRole);
    } else if (OJServerIDSet.contains(ID)) {
        json.insert("role", Global::OJServerRole);
    } else {
        Q_UNREACHABLE();
    }
    return handled;
}

void BCServerNetwork::handleNetworkConfigAction(const QJsonObject& json)
{
    Q_ASSERT(json["action"] == "network_config");
    const auto& type = json["type"].toString();
    if (type == "handshake") {
        auto        ID   = getID(json);
        const auto& role = json["role"].toString();
        if (role == Global::serverRole) {
            Q_ASSERT(!serverIDSet.contains(ID));
            serverIDSet.insert(ID);
        } else if (role == Global::OJServerRole) {
            Q_ASSERT(!OJServerIDSet.contains(ID));
            OJServerIDSet.insert(ID);
        } else {
            Q_UNREACHABLE();
        }
    } else {
        Q_UNREACHABLE();
    }
}

void BCServerNetwork::listen()
{
    const auto& role   = Global::Local::role;
    const auto& config = Global::config;
    Q_ASSERT(config.contains(role));
    Q_ASSERT(config[role].toObject().contains("port"));
    auto ip = config[role].toObject()["port"].toInt();
    listenTo(static_cast<quint16>(ip));
}
