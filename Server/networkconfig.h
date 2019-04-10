#ifndef NETWORKCONFIG_H
#define NETWORKCONFIG_H

#include <QByteArray>
#include <QJsonObject>
#include <QList>
#include <QObject>
#include <QString>

class TcpSocket;

class NetworkConfig : public QObject
{
    Q_OBJECT
public:
    struct Host
    {
        QString ip;
        quint16 port;
        QString user;
    };

public:
    explicit NetworkConfig(QObject* parent = nullptr);
    void updateServer();
    Host nextOJServer() const;
    Host nextBackEndServer() const;
    bool isValid() const;
private slots:
    void readyToReceive(const QJsonObject& json);

private:
    void send(const QJsonObject& json);

private:
    TcpSocket* socket;
    Host       ojServer;
    Host       backEndServer;
    bool       valid;
};

#endif  // NETWORKCONFIG_H
