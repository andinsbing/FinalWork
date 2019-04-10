#include "textlog.h"
#include <QDate>
#include <QDir>
#include <QFileInfo>
#include <QMutex>
#include <QString>
#include <QTime>
#include <QtGlobal>

// multithread security
void writeMessage(const QString& str)
{
    static QMutex mutex;
    mutex.lock();

    static const QString logDir  = "log/textlog";
    auto                 logPath = logDir + QDate::currentDate().toString("yyMMdd");
    QFileInfo            info(logPath);
    auto                 dir = info.absoluteDir();
    if (!dir.exists()) {
        dir.mkpath(dir.path());
    }
    Q_ASSERT(dir.exists());
    QFile file(logPath);
    bool  opened = file.open(QIODevice::WriteOnly | QIODevice::Append);
    Q_ASSERT(opened);
    auto writed = file.write(str.toUtf8());
    Q_ASSERT(writed != -1);
    file.close();

    mutex.unlock();
}

void defaultMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    auto info =
        QString(
            "Time: %1\nMessage: %2\nFile: %3 Line: %4\nFuction: %5\n--------------------------\n")
            .arg(QTime::currentTime().toString("HH.mm.ss"))
            .arg(msg)
            .arg(context.file)
            .arg(context.line)
            .arg(context.function);
    switch (type) {
    case QtDebugMsg:
        writeMessage(QString("Type: Debug\n") + info);
        break;
    case QtInfoMsg:
        writeMessage(QString("Type: Info\n") + info);
        break;
    case QtWarningMsg:
        writeMessage(QString("Type: Warning\n") + info);
        break;
    case QtCriticalMsg:
        writeMessage(QString("Type: Critical\n") + info);
        break;
    case QtFatalMsg:
        writeMessage(QString("Type: Fatal\n") + info);
        break;
    default:
        Q_UNREACHABLE();
        break;
    }
}

void TextLog::resetMessageHandler()
{
    qInstallMessageHandler(defaultMessageOutput);
}
