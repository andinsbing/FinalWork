#ifndef CLIENTMODEL_H
#define CLIENTMODEL_H

#include "../library/network/networkmodel.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <QObject>
#include <QVector>

class ClientModel : public NetworkModel
{
    Q_OBJECT
public:
    explicit ClientModel(QObject* parent = nullptr);

signals:
    void sendJson(QJsonObject json);
signals:
    void waitForLogin();
    void loginResult(bool ok, const QString& hint = "");
    void submitResult(bool ok);
    void taskFinished(const QJsonObject& json);
    void updateName(const QString& name);
    void updateTopicArray(const QJsonArray& array);

public:
    void start() override;

public:
    void newLoginEvent(const QJsonObject& json);
    void newInitEvent(const QJsonObject& json);
    void newInfoEvent(const QJsonObject& json);
    void newPushEvent(const QJsonObject& json);

public:
    void acceptLoginResult(const QJsonObject& json);
    void acceptSubmitResult(const QJsonObject& json);

public:
    void login(const QString& name, const QString& password);
    void submit(int index, const QString& code, const QString& language);
    void info(const QString& text);

public:
    void updateRank(const QJsonObject& json);
    void updateTopic(const QJsonObject& json);

private:
    QString     name;
    QString     passwrod;
    QJsonObject topic;
    QJsonObject rank;
    QJsonArray  task;
};

#endif  // CLIENTMODEL_H
