#ifndef NETWORKMODEL_H
#define NETWORKMODEL_H

#include <QJsonObject>
#include <QObject>
#include <QString>

class NetworkModel : public QObject
{
    Q_OBJECT
signals:
    void sendJson(const QJsonObject& json);

public:
    explicit NetworkModel(QObject* parent = nullptr);

protected:
    bool    isClientJson(const QJsonObject& json) const;
    bool    isServerJson(const QJsonObject& json) const;
    bool    isBCServerJson(const QJsonObject& json) const;
    bool    isOJServerJson(const QJsonObject& json) const;
    QString getRole(const QJsonObject& json) const;
    void    denialOfService(const QJsonObject& json) const;
    void    denialOfService() const;
    void    denialOfService(const QString& tip) const;

public slots:
    virtual void start() = 0;
};

#endif  // NETWORKMODEL_H
