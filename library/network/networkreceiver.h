#ifndef NETWORKRECEIVER_H
#define NETWORKRECEIVER_H

#include <QJsonObject>
#include <QObject>
#include <QString>

class NetworkReceiver : public QObject
{
    Q_OBJECT
public:
    explicit NetworkReceiver(QObject* parent = nullptr);
signals:
    void newRequestAction(const QJsonObject& json, const QString& type);
    void newCommandAction(const QJsonObject& json, const QString& type);
    void newPushAction(const QJsonObject& json, const QString& type);
    void newReplyAction(const QJsonObject& json, const QString& type);
    void newPullAction(const QJsonObject& json, const QString& type);

public slots:
    void acceptJson(const QJsonObject& json);

public:
    void setNetwork(class Network* network);
};

#endif  // NETWORKRECEIVER_H
