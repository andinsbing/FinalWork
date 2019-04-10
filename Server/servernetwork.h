#ifndef SERVERNETWORK_H
#define SERVERNETWORK_H

#include "../library/network/network.h"
#include <QSet>

class ServerNetwork : public Network
{
    Q_OBJECT
public:
    explicit ServerNetwork(QObject* parent = nullptr);

public slots:
    // assume json is prue data and not contain network id
    void sendJsonToBCServer(QJsonObject json);
    void broadcastToClient(QJsonObject json);
    void start() override;

protected:
    bool PreprocessReceivedJson(QJsonObject& json) override;

private:
    void listen();
    void connectToBCServer();
    void handleNetworkConfigAction(const QJsonObject& json);

private:
    qint32       BCServerNetworkID;
    QSet<qint32> clientIDSet;
    QSet<qint32> backEndIDSet;
};

#endif  // SERVERNETWORK_H
