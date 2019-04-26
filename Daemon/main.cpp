#include "../library/file/filesystem.h"
#include "../library/global_config/globalconfig.h"
#include "../library/json/jsonfunction.h"
#include <QCoreApplication>
#include <QDebug>
#include <QObject>
#include <QProcess>
#include <QThread>
#include <QTime>
#include <QTimer>

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    Q_ASSERT(JsonFunction::hasKey(Global::config, "app"));
    auto path = Global::config["app"].toString();
    Q_ASSERT(FileSystem().hasFile(path));

    QProcess p;
    p.setProgram(path);

    QObject::connect(&p, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                     [&p](int exitCode, QProcess::ExitStatus exitStatus) {
                         if (exitCode != 0) {
                             qDebug() << QTime::currentTime().toString()
                                      << "program exited, exit code:" << exitCode
                                      << ",exit status:" << exitStatus;
                         }
                         QThread::msleep(1000);
                         p.start();
                         qDebug() << QTime::currentTime().toString() << "program restarted";
                     });
    QObject::connect(&p, &QProcess::errorOccurred, [&p](QProcess::ProcessError error) {
        QThread::msleep(1000);
        qDebug() << QTime::currentTime().toString() << "program error occurred:" << error;
        p.start();
        qDebug() << QTime::currentTime().toString() << "program restarted";
    });

    p.start();
    qDebug() << QTime::currentTime().toString() << path << "program started";
    return a.exec();
}
