#ifndef SQLCONTROLLER_H
#define SQLCONTROLLER_H

#include <QJsonObject>
#include <QList>
#include <QObject>

class SQLController : public QObject
{
    Q_OBJECT
public:
    explicit SQLController(QObject* parent = nullptr);

signals:
    void updateTopic(const QJsonObject& json);
    void updateUser(const QJsonObject& json);
    void updateContest(const QJsonObject& json);

public slots:
    void getContest();

private:
    QJsonObject getUser();
    QJsonObject getTopic(int id);

public:
    void addTopic(const QJsonObject& json);

private:
    QJsonArray readTable(const QString& command);

private:
    QJsonArray getTable(const QString& tableName);
    QJsonArray getTable(const QString& tableName, const QString& where);

public slots:
    void start();

private:
    class SQLConnector* connector;
};

#endif  // SQLCONTROLLER_H
