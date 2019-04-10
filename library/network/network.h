#ifndef NETWORK_H
#define NETWORK_H

#include "../data_structure/doublemap.h"
#include <QJsonObject>
#include <QObject>
#include <QSet>
#include <QVector>

class TcpSocket;
class TcpServer;

/*assume json format
 *@network:id -> message sender or receiver
 */
class Network : public QObject
{
    Q_OBJECT
public:
    explicit Network(QObject* parent = nullptr);

signals:
    void jsonRecived(const QJsonObject& jsonReceived);
    void networkDisconnected(qint32 networkID);

public:
public slots:
    void         sendJson(QJsonObject json);
    virtual void start() = 0;

private slots:
    void acceptConnection(TcpSocket* socket);
    void acceptJson(QJsonObject json, TcpSocket* sender);
    void socketDisconnected(TcpSocket* socket);

protected:
    virtual bool PreprocessReceivedJson(QJsonObject& json) = 0;

protected:
    bool   hasID(const QJsonObject& json) const;
    void   markSpecificID(QJsonObject& json, qint32 ID);
    void   unmarkID(QJsonObject& json);
    qint32 getID(const QJsonObject& json);
    bool   isValidNetwork(qint32 networkID);
    bool   isValidNetwork(const QJsonObject& json);

private:
    void   markID(QJsonObject& json, TcpSocket* socket);
    qint32 generateID() const;

protected:
    // create new socket, connect to host, return ID
    qint32 connectToHost(const QString& hostName, const quint16 port);
    void   listenTo(quint16 port);

private:
    TcpServer*                    server;
    DoubleMap<TcpSocket*, qint32> socketIDMap;
    QSet<qint32>                  abnormalSocket;
};

#endif  // NETWORK_H
