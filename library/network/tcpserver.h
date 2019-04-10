#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
class QTcpServer;
class TcpSocket;

class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject* parent = nullptr);
    void listen(quint16 port);
signals:
    void newConnection(TcpSocket* socket);
private slots:
    void acceptConnection();

private:
    QTcpServer* server;
};

#endif  // TCPSERVER_H
