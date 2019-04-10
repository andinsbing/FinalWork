#ifndef SQLCONNECTOR_H
#define SQLCONNECTOR_H

#include <QObject>
#include <QSqlDatabase>

class SQLConnector : public QObject
{
    Q_OBJECT
public:
    explicit SQLConnector(QObject* parent = nullptr);

signals:

public:
    void start();

private:
    QSqlDatabase SQLDB;
};

#endif  // SQLCONNECTOR_H
