#ifndef CLIENTNETWORK_H
#define CLIENTNETWORK_H

#include "../library/network/network.h"

class ClientNetwork : public Network
{
public:
    ClientNetwork(QObject* parent = nullptr);

public slots:
    void sendJsonToServer(QJsonObject json);
    void start() override;

protected:
    bool PreprocessReceivedJson(QJsonObject& json) override;

private:
    void connetToServer();

private:
    qint32 serverNetwrokID;
};

#endif  // CLIENTNETWORK_H
