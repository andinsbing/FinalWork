#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H

#include <QJsonObject>
#include <QString>

namespace Global
{
extern const QJsonObject config;
static const QString     clientRole   = "client";
static const QString     serverRole   = "server";
static const QString     BCServerRole = "bc_server";
static const QString     OJServerRole = "oj_server";

namespace Local
{
#ifdef SERVER
    static const QString role = serverRole;
#endif
#ifdef BC_SERVER
    static const QString role = BCServerRole;
#endif
#ifdef CLIENT
    static const QString role = clientRole;
#endif
#ifdef OJ_SERVER
    static const QString role = OJServerRole;
#endif
}  // namespace Local

}  // namespace Global

#endif  // GLOBALCONFIG_H
