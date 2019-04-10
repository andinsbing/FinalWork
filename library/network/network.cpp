#include "network.h"
#include "../library/global_config/globalconfig.h"
#include "tcpserver.h"
#include "tcpsocket.h"

Network::Network(QObject* parent) : QObject(parent), server(new TcpServer(this)), socketIDMap{}
{
    connect(server, &TcpServer::newConnection, this, &Network::acceptConnection);
}

void Network::sendJson(QJsonObject json)
{
    Q_ASSERT(hasID(json));
    auto socket = socketIDMap[getID(json)];
    unmarkID(json);
    socket->write(json);
}

bool Network::isValidNetwork(qint32 networkID)
{
    Q_ASSERT(socketIDMap.has(networkID));
    return socketIDMap[networkID]->isValid();
}

bool Network::isValidNetwork(const QJsonObject& json)
{
    return isValidNetwork(getID(json));
}

qint32 Network::connectToHost(const QString& hostName, const quint16 port)
{
    auto socket = new TcpSocket(this);
    socket->connectToHost(hostName, port);
    connect(socket, &TcpSocket::jsonReceived, this, &Network::acceptJson);
    connect(socket, &TcpSocket::disconnected, this, &Network::socketDisconnected);
    auto ID = this->generateID();
    socketIDMap.update(socket, ID);
    return ID;
}

void Network::listenTo(quint16 port)
{
    server->listen(port);
}

void Network::acceptConnection(TcpSocket* socket)
{
    socketIDMap.update(socket, generateID());
    connect(socket, &TcpSocket::jsonReceived, this, &Network::acceptJson);
    connect(socket, &TcpSocket::disconnected, this, &Network::socketDisconnected);
}

void Network::acceptJson(QJsonObject json, TcpSocket* sender)
{
    Q_ASSERT(!hasID(json));
    markID(json, sender);
    bool handled = PreprocessReceivedJson(json);
    if (!handled) {
        emit jsonRecived(json);
    }
}

void Network::socketDisconnected(TcpSocket* socket)
{
    Q_ASSERT(socketIDMap.has(socket));
    auto ID = socketIDMap[socket];
    Q_ASSERT(!abnormalSocket.contains(ID));
    abnormalSocket.insert(ID);
    emit networkDisconnected(ID);
}

bool Network::hasID(const QJsonObject& json) const
{
    return json.contains("network_id");
}

void Network::markID(QJsonObject& json, TcpSocket* socket)
{
    markSpecificID(json, socketIDMap[socket]);
}

void Network::markSpecificID(QJsonObject& json, qint32 ID)
{
    Q_ASSERT(!hasID(json));
    Q_ASSERT(socketIDMap.has(ID));
    json["network_id"] = ID;
}

void Network::unmarkID(QJsonObject& json)
{
    Q_ASSERT(hasID(json));
    json.remove("network_id");
}

qint32 Network::getID(const QJsonObject& json)
{
    Q_ASSERT(hasID(json));
    return json["network_id"].toInt();
}

qint32 Network::generateID() const
{
    static qint32 ID = 0;
    return ++ID;
}
