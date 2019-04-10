#include "tcpserver.h"
#include "tcpsocket.h"
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>

TcpServer::TcpServer(QObject* parent) : QObject(parent), server(new QTcpServer(this))
{
    connect(server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
}

void TcpServer::acceptConnection()
{
    auto nextConnection = server->nextPendingConnection();
    qDebug() << "accept connection, ip:" << nextConnection->peerAddress()
             << " port:" << nextConnection->peerPort();
    emit newConnection(new TcpSocket(nextConnection, this));
}

void TcpServer::listen(quint16 port)
{
    if (!server->listen(QHostAddress::AnyIPv4, port)) {
        qDebug() << "tcp listen error" << server->errorString();
    }
}
