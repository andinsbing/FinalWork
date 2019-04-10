#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <QJsonObject>
#include <QObject>
#include <initializer_list>

class FileSystem : public QObject
{
    Q_OBJECT
public:
    explicit FileSystem(QObject* parent = nullptr);
    void        makeFile(const QString& path, const QByteArray& data);
    QByteArray  readFile(const QString& path);
    QJsonObject readJson(const QString& path);
    void        makeJson(const QString& path, const QJsonObject& json);
    bool        hasDIr(const QString& dir);
    bool        hasDIr(const std::initializer_list<QString>& dirList);
    bool        hasFile(const QString& path);
    bool        hasFile(const std::initializer_list<QString>& pathList);
};

#endif  // FILESYSTEM_H
