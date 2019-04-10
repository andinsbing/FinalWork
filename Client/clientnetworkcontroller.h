#ifndef CLIENTNETWORKCONTROLLER_H
#define CLIENTNETWORKCONTROLLER_H

#include "../library/network/networkcontroller.h"
#include <QJsonObject>
#include <QObject>

class ClientNetworkController : public NetworkController
{
    Q_OBJECT
public:
    explicit ClientNetworkController(QObject* parent = nullptr);
    void SetModel(class ClientModel* clientModel);

protected:
    void handleRequestAction(const QJsonObject& json, const QString& type) override;
    void handleCommandAction(const QJsonObject& json, const QString& type) override;
    void handlePushAction(const QJsonObject& json, const QString& type) override;
    void handleReplyAction(const QJsonObject& json, const QString& type) override;
    void handlePullAction(const QJsonObject& json, const QString& type) override;

private:
    class ClientModel* model;
};

#endif  // CLIENTNETWORKCONTROLLER_H
