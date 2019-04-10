#ifndef BCSERVERNETWORKMODE_H
#define BCSERVERNETWORKMODE_H

#include "../library/network/networkmodel.h"
#include <QList>
#include <QTime>

class BCServerNetworkModel : public NetworkModel
{
    Q_OBJECT
public:
    explicit BCServerNetworkModel(QObject* parent = nullptr);

signals:
    void broadcastToServer(const QJsonObject& json);
    void broadcastToOJServer(const QJsonObject& json);
    void newTask(const QJsonObject& json);
    void waitForContestData();

public slots:
    void updateTopic(const QJsonObject& json);
    void updateUser(const QJsonObject& json);
    void updateContest(const QJsonObject& json);

public:
    void newPullEvent(const QJsonObject& json);
    void newPushEvent(const QJsonObject& json);
    void newSubmitEvent(const QJsonObject& json);

public slots:
    void start();
    void networkDisconnected(qint32 networkID);

private:
    void addTestSubmit();
    void dispatchTask();

private:
    QJsonObject               contest;
    QJsonObject               user;
    QJsonObject               topic;
    class TaskManager*        taskManager;
    QList<qint32>             OJReadyLIst;
    QMap<qint32, QJsonObject> OJTask;
};

#endif  // BCSERVERNETWORKMODE_H
