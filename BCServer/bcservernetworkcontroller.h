#ifndef BCSERVERNETWORKCONTROLLER_H
#define BCSERVERNETWORKCONTROLLER_H

#include "../library/network/networkcontroller.h"

class BCServerNetworkController : public NetworkController
{
    Q_OBJECT
public:
    explicit BCServerNetworkController(QObject* parent = nullptr);

public:
    void setModel(class BCServerNetworkModel* model);
protected slots:
    void handleRequestAction(const QJsonObject& json, const QString& type) override;
    void handleCommandAction(const QJsonObject& json, const QString& type) override;
    void handlePushAction(const QJsonObject& json, const QString& type) override;
    void handleReplyAction(const QJsonObject& json, const QString& type) override;
    void handlePullAction(const QJsonObject& json, const QString& type) override;

private:
    class BCServerNetworkModel* model = nullptr;
};

#endif  // BCSERVERNETWORKCONTROLLER_H
