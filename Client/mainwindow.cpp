#include "mainwindow.h"
#include "../library/global_config/globalconfig.h"
#include "clientmodel.h"
#include "clientnetwork.h"
#include "clientnetworkcontroller.h"
#include "clientviewcontroller.h"
#include "infodialog.h"
#include "logindialog.h"
#include "submitdialog.h"
#include "taskdialog.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QThread>
#include <QTimer>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), network(new ClientNetwork),
      networkController(new ClientNetworkController(this)),
      viewController(new ClientViewController(this)), model(new ClientModel(this)),
      loginDialog(new LoginDialog(this)), submitDialog(new SubmitDialog(this)),
      thread(new QThread(this)), infoDialog(new InfoDialog(this)), taskDialog(new TaskDialog(this))
{
    ui->setupUi(this);
    network->start();
    network->moveToThread(thread);
    thread->start();
    //    QThread::msleep(1000);
    //    QTimer::singleShot(1000, network, &ClientNetwork::start);

    networkController->setNetwork(network);
    networkController->SetModel(model);
    viewController->SetModel(model);

    connect(model, &ClientModel::sendJson, network, &ClientNetwork::sendJsonToServer);

    // login
    connect(model, &ClientModel::waitForLogin, [this] {
        this->hide();
        QTimer::singleShot(100, loginDialog, &LoginDialog::start);
    });
    connect(loginDialog, &LoginDialog::login, viewController, &ClientViewController::login);
    connect(model, &ClientModel::loginResult, [this](bool ok, const QString& hint) {
        loginDialog->acceptLoginResult(ok, hint);
        if (ok) {
            this->show();
        }
    });
    connect(loginDialog, &LoginDialog::finished, [] { qApp->quit(); });

    // submit
    connect(model, &ClientModel::submitResult, [this](bool ok) {
        if (ok) {
            QMessageBox::information(this, "submit result", "accept");
        } else {
            QMessageBox::information(this, "submit result", "deny");
        }
    });
    connect(submitDialog, &SubmitDialog::submit, viewController, &ClientViewController::submit);
    connect(ui->submitPushButton, &QPushButton::clicked, submitDialog, &SubmitDialog::start);

    // info
    connect(infoDialog, &InfoDialog::info, viewController, &ClientViewController::info);
    connect(ui->infoPushButton, &QPushButton::clicked, infoDialog, &InfoDialog::start);

    // task info
    connect(model, &ClientModel::taskFinished, taskDialog, &TaskDialog::addTask);
    connect(ui->taskPushButton, &QPushButton::clicked, this->taskDialog, &TaskDialog::show);

    //    model->start();
    QTimer::singleShot(10, model, &ClientModel::start);
}

MainWindow::~MainWindow()
{
    thread->quit();
    delete ui;
}

void MainWindow::test()
{
    void* p = nullptr;
    Q_UNUSED(p);
    this->close();
}
