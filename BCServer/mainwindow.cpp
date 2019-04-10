#include "mainwindow.h"
#include "bcservernetwork.h"
#include "bcservernetworkcontroller.h"
#include "bcservernetworkmodel.h"
#include "sqlcontroller.h"
#include "taskmanager.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QThread>
#include <QTimer>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), network(new BCServerNetwork),
      networkThread(new QThread(this)), netowrkController(new BCServerNetworkController(this)),
      networkModel(new BCServerNetworkModel(this)), sqlController(new SQLController),
      sqlThread(new QThread(this))
{
    ui->setupUi(this);
    network->start();
    network->moveToThread(networkThread);
    networkThread->start();

    // controller
    netowrkController->setNetwork(network);
    netowrkController->setModel(networkModel);

    // model
    connect(networkModel, &BCServerNetworkModel::sendJson, network, &BCServerNetwork::sendJson);
    connect(networkModel, &BCServerNetworkModel::broadcastToServer, network,
            &BCServerNetwork::broadcastToServer);
    connect(network, &BCServerNetwork::networkDisconnected, networkModel,
            &BCServerNetworkModel::networkDisconnected);

    // sql
    sqlController->moveToThread(sqlThread);
    connect(sqlController, &SQLController::updateUser, networkModel,
            &BCServerNetworkModel::updateUser);
    connect(sqlController, &SQLController::updateTopic, networkModel,
            &BCServerNetworkModel::updateTopic);
    connect(sqlController, &SQLController::updateContest, networkModel,
            &BCServerNetworkModel::updateContest);
    connect(networkModel, &BCServerNetworkModel::waitForContestData, sqlController,
            &SQLController::getContest);

    sqlThread->start();

    QTimer::singleShot(100, sqlController, &SQLController::start);
    QTimer::singleShot(1000, networkModel, &BCServerNetworkModel::start);
}

MainWindow::~MainWindow()
{
    networkThread->quit();
    sqlThread->quit();

    delete ui;
}
