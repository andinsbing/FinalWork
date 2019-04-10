#include "ojservernetworkcontroller.h"
#include "ojservernetworkmodel.h"

OJServerNetworkController::OJServerNetworkController(QObject* parent) : NetworkController(parent) {}

void OJServerNetworkController::handleRequestAction(const QJsonObject& json, const QString& type)
{
    Q_UNUSED(json);
    Q_UNUSED(type);
    Q_UNREACHABLE();
}

void OJServerNetworkController::handleCommandAction(const QJsonObject& json, const QString& type)
{
    Q_UNUSED(json);
    Q_UNUSED(type);
    Q_UNREACHABLE();
}

void OJServerNetworkController::handlePushAction(const QJsonObject& json, const QString& type)
{
    Q_UNUSED(type);
    model->newPushEvent(json);
}

void OJServerNetworkController::handleReplyAction(const QJsonObject& json, const QString& type)
{
    Q_UNUSED(json);
    Q_UNUSED(type);
    Q_UNREACHABLE();
}

void OJServerNetworkController::handlePullAction(const QJsonObject& json, const QString& type)
{
    Q_UNUSED(json);
    Q_UNUSED(type);
    Q_UNREACHABLE();
}

void OJServerNetworkController::setModel(OJServerNetworkModel* networkMode)
{
    model = networkMode;
}
