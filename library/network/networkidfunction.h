#ifndef NETWORKIDFUNCTION_H
#define NETWORKIDFUNCTION_H

#include <QJsonObject>

struct NetworkIDFunction
{
    static qint32 getID(const QJsonObject& json);
    static void   setID(QJsonObject& json, qint32 ID);
    static void   copyID(QJsonObject& des, const QJsonObject& src);
    static bool   hasID(const QJsonObject& json);
    static void   removeID(QJsonObject& json);
    static void   replaceID(QJsonObject& json, qint32 ID);
};

#endif  // NETWORKIDFUNCTION_H
