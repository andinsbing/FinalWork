#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QAbstractSocket>
#include <QByteArray>
#include <QJsonObject>
#include <QObject>
#include <QString>
class QTcpSocket;

// provide a reliable network for json transmission
class TcpSocket : public QObject
{
    Q_OBJECT
public:
    explicit TcpSocket(QObject* parent = nullptr);
    explicit TcpSocket(QTcpSocket* socket, QObject* parent = nullptr);
    void    write(QJsonObject json);
    void    connectToHost(const QString& hostName, const quint16 port);
    QString localAddress() const;
    quint16 localPort() const;
    QString peerAddress() const;
    bool    isValid() const;

    quint16 peerPort() const;
signals:
    void jsonReceived(const QJsonObject& json, TcpSocket* sender);
    void disconnected(TcpSocket* sender);

private:
    const QJsonObject& localInfo() const;

private slots:
    void write(const QByteArray& data);
    void error(const QAbstractSocket::SocketError& info);
    void receiveByteArray();

private:
    QTcpSocket* socket;
    QByteArray  receptionBuffer;
    bool        hasError = false;
};

#endif  // TCPSOCKET_H
