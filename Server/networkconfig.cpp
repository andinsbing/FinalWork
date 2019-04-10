#include "networkconfig.h"
#include "../library/network/tcpsocket.h"
#include <QHostInfo>

NetworkConfig::NetworkConfig(QObject* parent)
    : QObject(parent), socket(new TcpSocket(this)), valid(false)
{
    this->connect(socket, SIGNAL(jsonReceived(QJsonObject, TcpSocket*)), this,
                  SLOT(readyToReceive(QJsonObject)));
    //    socket->connectToUtilityServer();
    this->updateServer();
}

void NetworkConfig::updateServer()
{
    valid = false;
    QJsonObject json;
    json.insert("action", "query");
    json.insert("user", "server");
    send(json);
}

NetworkConfig::Host NetworkConfig::nextOJServer() const
{
    return this->ojServer;
}

NetworkConfig::Host NetworkConfig::nextBackEndServer() const
{
    return this->backEndServer;
}

bool NetworkConfig::isValid() const
{
    return valid;
}

void NetworkConfig::readyToReceive(const QJsonObject& json)
{
    Host host;
    //    host.ip   = json.value("ip").toString();
    //    host.user = json.value("user").toString();
    //    if (host.user == Global::OJServer::user) {
    //        host.port      = Global::OJServer::port;
    //        this->ojServer = host;
    //    } else if (host.user == Global::BackEndServer::user) {
    //        host.port           = Global::BackEndServer::port;
    //        this->backEndServer = host;
    //    } else {
    //        Q_ASSERT(false);
    //        Q_UNREACHABLE();
    //    }
    valid = true;
}

void NetworkConfig::send(const QJsonObject& json)
{
    socket->write(json);
}
