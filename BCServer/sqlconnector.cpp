#include "sqlconnector.h"
#include "../library/global_config/globalconfig.h"
#include <QDebug>
#include <QSqlError>

SQLConnector::SQLConnector(QObject* parent) : QObject(parent) {}

void SQLConnector::start()
{
    auto sqlJson = Global::config["mysql"].toObject();

    Q_ASSERT(!sqlJson.empty());

    auto host     = sqlJson["host"].toString();
    auto port     = sqlJson["port"].toInt();
    auto db       = sqlJson["db"].toString();
    auto type     = sqlJson["type"].toString();
    auto user     = sqlJson["user"].toString();
    auto password = sqlJson["password"].toString();

    SQLDB = QSqlDatabase::addDatabase(type);

    SQLDB.setDatabaseName(db);
    SQLDB.setPort(port);
    SQLDB.setHostName(host);
    SQLDB.setPassword(password);
    SQLDB.setUserName(user);

    bool isOpened = SQLDB.open();
    if (!isOpened) {
        qDebug() << SQLDB.lastError();
    }
    Q_ASSERT(isOpened);
}
