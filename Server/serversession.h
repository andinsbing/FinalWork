#ifndef SERVERSESSION_H
#define SERVERSESSION_H

#include "../library/network/session.h"

struct ServerSessionData
{
    qint32  networkID = -1;
    qint32  userID    = -1;
    QString name;
    QString password;
};

class ServerSession : public QObject, public Session<ServerSessionData>
{
    Q_OBJECT
public:
    explicit ServerSession(QObject* parent = nullptr);
};

#endif  // SERVERSESSION_H
