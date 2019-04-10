#include "clientviewcontroller.h"
#include "clientmodel.h"
ClientViewController::ClientViewController(QObject* parent) : QObject(parent) {}

void ClientViewController::SetModel(ClientModel* clientModel)
{
    this->model = clientModel;
}

void ClientViewController::login(const QString& name, const QString& password)
{
    model->login(name, password);
}

void ClientViewController::submit(int index, const QString& code, const QString& language)
{
    model->submit(index, code, language);
}

void ClientViewController::info(const QString& text)
{
    model->info(text);
}
