#ifndef SESSION_H
#define SESSION_H

#include "networkidfunction.h"
#include <QHash>
#include <QList>

// cooperate with network id
// follow assumption of network id

template <class T>
class Session
{
public:
    Session()          = default;
    virtual ~Session() = default;
    T* createSession(const QJsonObject& json)
    {
        return createSession(NetworkIDFunction::getID(json));
    }
    T* createSession(qint32 ID)
    {
        Q_ASSERT(!hasSession(ID));
        sessionPool.append(T{});
        auto pSession   = &sessionPool.last();
        sessionHash[ID] = pSession;
        return pSession;
    }

    bool hasSession(const QJsonObject& json) const
    {
        return hasSession(NetworkIDFunction::getID(json));
    }
    bool hasSession(qint32 ID) const
    {
        return sessionHash.contains(ID);
    }

    T* getSession(const QJsonObject& json) const
    {
        return getSession(NetworkIDFunction::getID(json));
    }
    T* getSession(qint32 ID) const
    {
        Q_ASSERT(sessionHash.contains(ID));
        return sessionHash[ID];
    }

    T* operator[](const QJsonObject& json)
    {
        return (*this)[NetworkIDFunction::getID(json)];
    }
    T* operator[](qint32 ID)
    {
        return hasSession(ID) ? getSession(ID) : createSession(ID);
    }

private:
    QHash<qint32, T*> sessionHash;
    QList<T>          sessionPool;
};

#endif  // SESSION_H
