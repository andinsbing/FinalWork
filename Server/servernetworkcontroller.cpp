#include "servernetworkcontroller.h"
#include "../library/global_config/globalconfig.h"
#include "servernetworkmodel.h"

ServerNetworkController::ServerNetworkController(QObject* parent) : NetworkController(parent) {}

void ServerNetworkController::handleRequestAction(const QJsonObject& json, const QString& type)
{
    if (type == "login") {
        model->newLoginEvent(json);
    } else if (type == "submit") {
        model->newSubmitEvent(json);
    } else {
        Q_UNREACHABLE();
    }
}

void ServerNetworkController::handleCommandAction(const QJsonObject& json, const QString& type)
{
    if (type == "info") {
        model->newInfoEvent(json);
    } else {
        Q_UNREACHABLE();
    }
}

void ServerNetworkController::handlePushAction(const QJsonObject& json, const QString& type)
{
    Q_UNUSED(type);
    model->newPushEvent(json);
}

void ServerNetworkController::handleReplyAction(const QJsonObject& json, const QString& type)
{
    NetworkController::handleReplyAction(json, type);
}

void ServerNetworkController::handlePullAction(const QJsonObject& json, const QString& type)
{
    Q_UNUSED(type);
    model->newPullEvent(json);
}

void ServerNetworkController::setModel(ServerNetworkModel* networkMode)
{
    this->model = networkMode;
}
