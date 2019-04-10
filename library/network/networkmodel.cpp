#include "networkmodel.h"
#include "../library/global_config/globalconfig.h"

NetworkModel::NetworkModel(QObject* parent) : QObject(parent) {}

bool NetworkModel::isClientJson(const QJsonObject& json) const
{
    return getRole(json) == Global::clientRole;
}

bool NetworkModel::isServerJson(const QJsonObject& json) const
{
    return getRole(json) == Global::serverRole;
}

bool NetworkModel::isBCServerJson(const QJsonObject& json) const
{
    return getRole(json) == Global::BCServerRole;
}

bool NetworkModel::isOJServerJson(const QJsonObject& json) const
{
    return getRole(json) == Global::OJServerRole;
}

QString NetworkModel::getRole(const QJsonObject& json) const
{
    Q_ASSERT(json.contains("role"));
    return json["role"].toString();
}

void NetworkModel::denialOfService(const QJsonObject& json) const
{
    Q_UNUSED(json);
    Q_UNREACHABLE();
}

void NetworkModel::denialOfService() const {}

void NetworkModel::denialOfService(const QString& tip) const
{
    Q_UNUSED(tip);
    Q_UNREACHABLE();
}
