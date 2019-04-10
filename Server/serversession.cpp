#include "serversession.h"

ServerSession::ServerSession(QObject* parent) : QObject(parent), Session<ServerSessionData>() {}
