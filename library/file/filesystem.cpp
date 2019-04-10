#include "filesystem.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>

FileSystem::FileSystem(QObject* parent) : QObject(parent) {}

void FileSystem::makeFile(const QString& path, const QByteArray& data)
{
    QFileInfo fileInfo(path);
    QDir      dir = fileInfo.absoluteDir();
    if (!dir.exists()) {
        bool isAbleToMakePath = dir.mkpath(dir.absolutePath());
        Q_ASSERT(isAbleToMakePath);
    }
    QFile file(path);
    bool  isAbleToWrite = file.open(QFile::WriteOnly);
    Q_ASSERT(isAbleToWrite);
    auto bytes = file.write(data);
    Q_ASSERT(bytes != -1);
    file.close();
}

QByteArray FileSystem::readFile(const QString& path)
{
    QFile file(path);
    bool  isAbleToRead = file.open(QFile::ReadOnly);
    Q_ASSERT(isAbleToRead);
    return file.readAll();
}

QJsonObject FileSystem::readJson(const QString& path)
{
    QJsonDocument doc = QJsonDocument::fromJson(readFile(path));
    Q_ASSERT(doc.isObject());
    return doc.object();
}

void FileSystem::makeJson(const QString& path, const QJsonObject& json)
{
    QJsonDocument doc;
    doc.setObject(json);
    makeFile(path, doc.toJson(QJsonDocument::JsonFormat::Indented));
}

bool FileSystem::hasDIr(const QString& dir)
{
    return QDir(dir).exists();
}

bool FileSystem::hasDIr(const std::initializer_list<QString>& dirList)
{
    for (const auto& i : dirList) {
        if (!hasDIr(i)) {
            return false;
        }
    }
    return true;
}

bool FileSystem::hasFile(const QString& path)
{
    return QFileInfo(path).exists();
}

bool FileSystem::hasFile(const std::initializer_list<QString>& pathList)
{
    for (const auto& i : pathList) {
        if (!hasFile(i)) {
            return false;
        }
    }
    return true;
}
