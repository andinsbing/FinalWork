#include "initialization.h"
#include "../log/textlog.h"

void Initialization::initializeALL()
{
    initializeTextLog();
}

void Initialization::initializeTextLog()
{
    TextLog::resetMessageHandler();
}
