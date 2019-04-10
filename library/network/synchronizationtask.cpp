#include "synchronizationtask.h"
#include <QFunctionPointer>
#include <QTimer>

SynchronizationTask::SynchronizationTask(int milliseconds, QObject* parent)
    : QObject(parent), timer(new QTimer(this))
{
    timer->setInterval(milliseconds);
    timer->setSingleShot(false);
}

void SynchronizationTask::start()
{
    Q_ASSERT(result.empty());
    Q_ASSERT(!query.empty());
    Q_ASSERT(queryFuction != nullptr);
    Q_ASSERT(checkFuction != nullptr);
    connect(timer, &QTimer::timeout, this, &SynchronizationTask::Process);
    queryFuction(query);
    timer->start();
}

void SynchronizationTask::Process()
{
    if (checkFuction(result)) {
        timer->stop();
        emit finish();
        return;
    }
}
