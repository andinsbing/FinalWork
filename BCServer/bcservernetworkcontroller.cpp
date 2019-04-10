#include "bcservernetworkcontroller.h"
#include "bcservernetworkmodel.h"

BCServerNetworkController::BCServerNetworkController(QObject* parent) : NetworkController(parent) {}

void BCServerNetworkController::setModel(BCServerNetworkModel* model)
{
    this->model = model;
}

void BCServerNetworkController::handleRequestAction(const QJsonObject& json, const QString& type)
{
    if (type == "submit") {
        model->newSubmitEvent(json);
    } else {
        Q_UNREACHABLE();
    }
}

void BCServerNetworkController::handleCommandAction(const QJsonObject& json, const QString& type)
{
    Q_UNREACHABLE();
}

void BCServerNetworkController::handlePushAction(const QJsonObject& json, const QString& type)
{
    Q_UNUSED(type);
    model->newPushEvent(json);
}

void BCServerNetworkController::handleReplyAction(const QJsonObject& json, const QString& type)
{
    Q_UNREACHABLE();
}

void BCServerNetworkController::handlePullAction(const QJsonObject& json, const QString& type)
{
    Q_UNUSED(type);
    model->newPullEvent(json);
}
