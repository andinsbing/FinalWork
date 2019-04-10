#ifndef OJSERVERNETWORKMODEL_H
#define OJSERVERNETWORKMODEL_H

#include "../library/network/networkmodel.h"

class OJServerNetworkModel : public NetworkModel
{
    Q_OBJECT
public:
    explicit OJServerNetworkModel(QObject* parent = nullptr);

signals:
    void sendJsonToBCServer(const QJsonObject& json);

public:
    void newPushEvent(const QJsonObject& json);

public:
    void start() override;

private:
    void onTaskReady(const QJsonObject& task);

private:
    class Judger* judger;
    class QTimer* timer;
    QJsonObject   topic;
};

#endif  // OJSERVERNETWORKMODEL_H
