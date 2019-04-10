#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui
{
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget* parent = nullptr);
    ~LoginDialog() override;

signals:
    void login(QString name, QString password);

public slots:
    void start();
    void acceptLoginResult(bool ok, const QString& hint);

private:
    void onLogin();

private:
    bool checkName(const QString& name);
    bool checkPassword(const QString& password);

private:
    Ui::LoginDialog* ui;
};

#endif  // LOGINDIALOG_H
