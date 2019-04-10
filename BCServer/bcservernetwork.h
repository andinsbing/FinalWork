#ifndef BCSERVERNETWORK_H
#define BCSERVERNETWORK_H

#include "../library/network/network.h"
#include <QSet>

class BCServerNetwork : public Network
{
    Q_OBJECT
public:
    explicit BCServerNetwork(QObject* parent = nullptr);

public slots:
    void start() override;
    void broadcastToServer(QJsonObject json);

protected:
    virtual bool PreprocessReceivedJson(QJsonObject& json) override;

protected:
    void handleNetworkConfigAction(const QJsonObject& json);

private:
    void listen();

private:
    QSet<qint32> serverIDSet;
    QSet<qint32> OJServerIDSet;
};

#endif  // BCSERVERNETWORK_H
