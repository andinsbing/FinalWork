#include "networkidfunction.h"

#define ID "network_id"

qint32 NetworkIDFunction::getID(const QJsonObject& json)
{
    Q_ASSERT(hasID(json));
    return json[ID].toInt();
}

void NetworkIDFunction::setID(QJsonObject& json, qint32 networkID)
{
    Q_ASSERT(!hasID(json));
    json.insert(ID, networkID);
}

void NetworkIDFunction::copyID(QJsonObject& des, const QJsonObject& src)
{
    Q_ASSERT(!hasID(des));
    Q_ASSERT(hasID(src));
    des.insert(ID, src[ID].toInt());
}

bool NetworkIDFunction::hasID(const QJsonObject& json)
{
    return json.contains(ID);
}

void NetworkIDFunction::removeID(QJsonObject& json)
{
    Q_ASSERT(hasID(json));
    json.remove(ID);
}

void NetworkIDFunction::replaceID(QJsonObject& json, qint32 networkID)
{
    Q_ASSERT(hasID(json));
    json.insert(ID, networkID);
}
