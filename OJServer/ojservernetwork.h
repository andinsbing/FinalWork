#ifndef OJSERVERNETWORK_H
#define OJSERVERNETWORK_H

#include "../library/network/network.h"
#include <QObject>

class OJServerNetwork : public Network
{
    Q_OBJECT
public:
    explicit OJServerNetwork(QObject* parent = nullptr);

public slots:
    void start() override;
    void sendJsonToBCServer(QJsonObject json);

protected:
    virtual bool PreprocessReceivedJson(QJsonObject& json) override;

private:
    void connectToBCServer();

private:
    qint32 BCServerNetworkID;
};

#endif  // OJSERVERNETWORK_H
