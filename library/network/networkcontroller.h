#ifndef NETWORKCONTROLLER_H
#define NETWORKCONTROLLER_H

#include <QJsonObject>
#include <QObject>
#include <QString>

class NetworkController : public QObject
{
    Q_OBJECT
public:
    explicit NetworkController(QObject* parent = nullptr);
    virtual ~NetworkController() = default;

protected slots:
    virtual void handleRequestAction(const QJsonObject& json, const QString& type);
    virtual void handleCommandAction(const QJsonObject& json, const QString& type);
    virtual void handlePushAction(const QJsonObject& json, const QString& type);
    virtual void handleReplyAction(const QJsonObject& json, const QString& type);
    virtual void handlePullAction(const QJsonObject& json, const QString& type);

public:
    void setNetwork(class Network* network);

private:
    class NetworkReceiver* networkReceiver = nullptr;
};

#endif  // NETWORKCONTROLLER_H
