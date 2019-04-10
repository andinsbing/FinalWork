#ifndef SYNCHRONIZATIONTASK_H
#define SYNCHRONIZATIONTASK_H

#include <QJsonObject>
#include <QObject>
#include <functional>

class QTimer;
class QThread;

class SynchronizationTask : public QObject
{
    Q_OBJECT

public:
    explicit SynchronizationTask(int milliseconds, QObject* parent = nullptr);

signals:
    void finish();

public slots:
    void start();

private slots:

private:
    void Process();

public:
    QJsonObject                      result;
    QJsonObject                      query;
    std::function<void(QJsonObject)> queryFuction = nullptr;
    std::function<bool(QJsonObject)> checkFuction = nullptr;

private:
    QTimer*  timer;
    QThread* thread;
};

#endif  // SYNCHRONIZATIONTASK_H
