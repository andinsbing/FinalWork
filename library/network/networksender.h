#ifndef NETWORKSENDER_H
#define NETWORKSENDER_H

#include <QObject>

class NetworkSender : public QObject
{
    Q_OBJECT
public:
    explicit NetworkSender(QObject* parent = nullptr);
signals:
    void sendJson(const QJsonObject& json);

public:
    void sendRequestActionJson(QJsonObject json);
    void sendPullActionJson(QJsonObject json);
    void sendPushActionJson(QJsonObject json);
    void sendReplyActionJson(QJsonObject json);
    void sendCammandActionJson(QJsonObject json);

public:
    void setNetwork(class Network* network);
};

#endif  // NETWORKSENDER_H
