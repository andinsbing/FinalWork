#include "networkcontroller.h"
#include "network.h"
#include "networkreceiver.h"

NetworkController::NetworkController(QObject* parent) : QObject(parent) {}

void NetworkController::handleRequestAction(const QJsonObject& json, const QString& type)
{
    Q_UNUSED(json);
    Q_UNUSED(type);
    Q_UNREACHABLE();
}

void NetworkController::handleCommandAction(const QJsonObject& json, const QString& type)
{
    Q_UNUSED(json);
    Q_UNUSED(type);
    Q_UNREACHABLE();
}

void NetworkController::handlePushAction(const QJsonObject& json, const QString& type)
{
    Q_UNUSED(json);
    Q_UNUSED(type);
    Q_UNREACHABLE();
}

void NetworkController::handleReplyAction(const QJsonObject& json, const QString& type)
{
    Q_UNUSED(json);
    Q_UNUSED(type);
    Q_UNREACHABLE();
}

void NetworkController::handlePullAction(const QJsonObject& json, const QString& type)
{
    Q_UNUSED(json);
    Q_UNUSED(type);
    Q_UNREACHABLE();
}

void NetworkController::setNetwork(Network* network)
{
    networkReceiver = new NetworkReceiver(this);
    networkReceiver->setNetwork(network);
    connect(networkReceiver, &NetworkReceiver::newPullAction, this,
            &NetworkController::handlePullAction);
    connect(networkReceiver, &NetworkReceiver::newPushAction, this,
            &NetworkController::handlePushAction);
    connect(networkReceiver, &NetworkReceiver::newReplyAction, this,
            &NetworkController::handleReplyAction);
    connect(networkReceiver, &NetworkReceiver::newCommandAction, this,
            &NetworkController::handleCommandAction);
    connect(networkReceiver, &NetworkReceiver::newRequestAction, this,
            &NetworkController::handleRequestAction);
}
