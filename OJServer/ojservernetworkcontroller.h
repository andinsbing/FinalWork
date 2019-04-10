#ifndef OJSERVERNETWORKCONTROLLER_H
#define OJSERVERNETWORKCONTROLLER_H

#include "../library/network/networkcontroller.h"

class OJServerNetworkController : public NetworkController
{
    Q_OBJECT
public:
    explicit OJServerNetworkController(QObject* parent = nullptr);

protected:
    void handleRequestAction(const QJsonObject& json, const QString& type) override;
    void handleCommandAction(const QJsonObject& json, const QString& type) override;
    void handlePushAction(const QJsonObject& json, const QString& type) override;
    void handleReplyAction(const QJsonObject& json, const QString& type) override;
    void handlePullAction(const QJsonObject& json, const QString& type) override;

public:
    void setModel(class OJServerNetworkModel* networkMode);

private:
    class OJServerNetworkModel* model = nullptr;
};

#endif  // OJSERVERNETWORKCONTROLLER_H
