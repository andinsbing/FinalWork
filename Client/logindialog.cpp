#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget* parent) : QDialog(parent), ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &LoginDialog::onLogin);
    connect(this, &LoginDialog::finished, qApp, QApplication::quit);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::start()
{
    ui->nameLineEdit->clear();
    ui->passwordLineEdit->clear();
    this->show();
}

void LoginDialog::acceptLoginResult(bool ok, const QString& hint)
{
    if (ok) {
        this->hide();
        QMessageBox::information(this, "login successed", "welcome");
    } else {
        QMessageBox::information(this, "login failed", hint);
        this->setEnabled(true);
    }
}

void LoginDialog::onLogin()
{
    const auto& name     = ui->nameLineEdit->text();
    const auto& password = ui->passwordLineEdit->text();
    if (!checkName(name) || !checkPassword(password)) {
        QMessageBox::information(this, "log failed", "name or password format is not valid");
    } else {
        this->setEnabled(false);
        emit login(name, password);
    }
}

bool LoginDialog::checkName(const QString& name)
{
    return name.length() > 0;
}

bool LoginDialog::checkPassword(const QString& password)
{
    return password.length() > 0;
}
