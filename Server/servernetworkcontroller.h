#ifndef SERVERNETWORKCONTROLLER_H
#define SERVERNETWORKCONTROLLER_H

#include "../library/network/networkcontroller.h"

class ServerNetworkController : public NetworkController
{
    Q_OBJECT
public:
    explicit ServerNetworkController(QObject* parent = nullptr);

protected:
    void handleRequestAction(const QJsonObject& json, const QString& type) override;
    void handleCommandAction(const QJsonObject& json, const QString& type) override;
    void handlePushAction(const QJsonObject& json, const QString& type) override;
    void handleReplyAction(const QJsonObject& json, const QString& type) override;
    void handlePullAction(const QJsonObject& json, const QString& type) override;

public:
    void setModel(class ServerNetworkModel* networkMode);

private:
    class ServerNetworkModel* model = nullptr;
};

#endif  // SERVERNETWORKCONTROLLER_H
