#ifndef CLIENTVIEWCONTROLLER_H
#define CLIENTVIEWCONTROLLER_H

#include <QJsonObject>
#include <QObject>

class ClientModel;

class ClientViewController : public QObject
{
    Q_OBJECT
public:
    explicit ClientViewController(QObject* parent = nullptr);
    void SetModel(ClientModel* clientModel);
signals:

public slots:
public:
    void login(const QString& name, const QString& password);
    void submit(int index, const QString& code, const QString& language);
    void info(const QString& text);

private:
    ClientModel* model;
};

#endif  // CLIENTVIEWCONTROLLER_H
