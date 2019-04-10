#include "clientnetworkcontroller.h"
#include "clientmodel.h"

ClientNetworkController::ClientNetworkController(QObject* parent) : NetworkController(parent) {}

void ClientNetworkController::SetModel(ClientModel* clientModel)
{
    this->model = clientModel;
}

void ClientNetworkController::handleRequestAction(const QJsonObject& json, const QString& type)
{
    NetworkController::handleRequestAction(json, type);
}

void ClientNetworkController::handleCommandAction(const QJsonObject& json, const QString& type)
{
    if (type == "login") {
        model->newLoginEvent(json);
    } else if (type == "init") {
        model->newInitEvent(json);
    } else if (type == "info") {
        model->newInfoEvent(json);
    } else {
        Q_UNREACHABLE();
    }
}

void ClientNetworkController::handlePushAction(const QJsonObject& json, const QString& type)
{
    Q_UNUSED(type);
    model->newPushEvent(json);
}

void ClientNetworkController::handleReplyAction(const QJsonObject& json, const QString& type)
{
    if (type == "login") {
        model->acceptLoginResult(json);
    } else if (type == "submit") {
        model->acceptSubmitResult(json);
    } else {
        Q_UNREACHABLE();
    }
}

void ClientNetworkController::handlePullAction(const QJsonObject& json, const QString& type)
{
    Q_UNUSED(type);
    Q_UNUSED(json);
    Q_UNREACHABLE();
}
