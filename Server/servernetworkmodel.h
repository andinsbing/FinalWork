#ifndef SERVERNETWORKMODEL_H
#define SERVERNETWORKMODEL_H

#include "../library/network/networkmodel.h"
#include "serversession.h"
#include <QJsonArray>
#include <QVector>

class ServerNetworkModel : public NetworkModel
{
    Q_OBJECT

private:
    struct Limit
    {
        int timeLimit;
        int memoryLImit;
    };

public:
    ServerNetworkModel(QObject* parent = nullptr);

signals:
    void sendJsonToBCServer(const QJsonObject& json);
    void broadcastToClient(const QJsonObject& json);

public:
    void newLoginEvent(const QJsonObject& json);
    void newPullEvent(const QJsonObject& json);
    void newSubmitEvent(const QJsonObject& json);
    void newInfoEvent(const QJsonObject& json);
    void newPushEvent(const QJsonObject& json);

public:
    void start() override;

protected:
private:
    void checkUser(const QString& name, const QString& password);
    void updateTopic(const QJsonObject& topicData);
    void updateUser(const QJsonObject& json);

private:
    QJsonObject           user;
    QJsonObject           topic;
    ServerSession*        session;
    QVector<Limit>        topicLimit;
    QJsonArray            taskFinished;
    QMap<qint32, QString> IDNameMap;
};

#endif  // SERVERNETWORKMODEL_H
