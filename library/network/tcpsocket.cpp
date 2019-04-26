#include "tcpsocket.h"
#include "../library/global_config/globalconfig.h"
#include <QDataStream>
#include <QDebug>
#include <QHostAddress>
#include <QJsonDocument>
#include <QTcpSocket>
#include <QThread>

struct JsonWrapper
{
    static bool       checkReady(const QByteArray& ba);
    static QByteArray wrap(const QJsonObject& json);
    // must check if ready before call
    // extact and remove data from ba
    static QJsonObject extractFrontJson(QByteArray& ba);

private:
    using SizeType                  = qint64;
    static constexpr auto dsVersion = QDataStream::Qt_5_12;
    static const SizeType minimumPackSize;
};

const JsonWrapper::SizeType JsonWrapper::minimumPackSize = [] {
    QByteArray            pack{};
    JsonWrapper::SizeType packSize{};
    QByteArray            content{};
    QDataStream           ds(&pack, QIODevice::WriteOnly);
    ds << packSize << content;
    return pack.size();
}();

QJsonObject JsonWrapper::extractFrontJson(QByteArray& ba)
{
    Q_ASSERT(checkReady(ba));

    QByteArray  content;
    SizeType    packSize = 0;
    QDataStream ds(ba);
    ds >> packSize >> content;
    ba.remove(0, static_cast<int>(packSize));
    return QJsonDocument::fromBinaryData(content).object();
}

bool JsonWrapper::checkReady(const QByteArray& ba)
{
    if (ba.size() < minimumPackSize) {
        return false;
    }
    QDataStream ds{ ba };
    SizeType    packSize = 0;
    ds >> packSize;
    return ba.size() >= packSize;
}

QByteArray JsonWrapper::wrap(const QJsonObject& json)
{
    QJsonDocument ducment{ json };
    auto          content = ducment.toBinaryData();
    QByteArray    pack;
    SizeType      packSIze{};
    QDataStream   ds{ &pack, QIODevice::WriteOnly };
    ds.setVersion(dsVersion);
    ds << packSIze << content;
    // rewrite packSize
    packSIze = pack.size();
    ds.device()->seek(0);
    ds << packSIze;
    return pack;
}

TcpSocket::TcpSocket(QObject* parent) : TcpSocket(new QTcpSocket, parent) {}

TcpSocket::TcpSocket(QTcpSocket* socket, QObject* parent)
    : QObject(parent), socket(socket), receptionBuffer()
{
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(error(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(readyRead()), this, SLOT(receiveByteArray()));
    socket->setParent(this);  // take ownship
}

void TcpSocket::write(const QByteArray& data)
{
    const auto& byteWriten = socket->write(data);
    Q_ASSERT(byteWriten != -1);
}

void TcpSocket::write(QJsonObject json)
{
    this->write(JsonWrapper::wrap(json));
}

void TcpSocket::connectToHost(const QString& hostName, const quint16 port)
{  // only allow ip4 protocol
    socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
    socket->connectToHost(hostName, port, QAbstractSocket::ReadWrite,
                          QAbstractSocket::IPv4Protocol);
    bool isConnected = socket->waitForConnected();
    if (!isConnected) {
        qDebug() << socket->error();
        Q_ASSERT_X(isConnected, "tcp connection", "cannot connet to host");
    }
}

QString TcpSocket::localAddress() const
{
    return this->socket->localAddress().toString();
}

quint16 TcpSocket::localPort() const
{
    return this->socket->localPort();
}

QString TcpSocket::peerAddress() const
{  // assune it's ip4 address
    return socket->peerAddress().toString();
}

bool TcpSocket::isValid() const
{
    return !hasError;
}

quint16 TcpSocket::peerPort() const
{
    return socket->peerPort();
}

const QJsonObject& TcpSocket::localInfo() const
{
    static QJsonObject info;
    if (info.empty()) {
        info.insert("ip", this->localAddress());
        info.insert("role", Global::Local::role);
        info.insert("port", QString::number(this->localPort()));
    }
    return info;
}

void TcpSocket::error(const QAbstractSocket::SocketError& info)
{
    hasError = true;
    qDebug() << __func__ << info;
    emit disconnected(this);
}

void TcpSocket::receiveByteArray()
{
    this->receptionBuffer.append(socket->readAll());
    while (JsonWrapper::checkReady(receptionBuffer)) {
        emit jsonReceived(JsonWrapper::extractFrontJson(receptionBuffer), this);
    }
}
