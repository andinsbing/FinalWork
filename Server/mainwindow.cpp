#include "mainwindow.h"
#include "../library/global_config/globalconfig.h"
#include "servernetwork.h"
#include "servernetworkcontroller.h"
#include "servernetworkmodel.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSqlQuery>
#include <QThread>
#include <QTimer>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow()), network(new ServerNetwork),
      thread(new QThread(this)), controller(new ServerNetworkController(this)),
      mode(new ServerNetworkModel(this))
{
    network->moveToThread(thread);
    thread->start();
    QTimer::singleShot(1000, network, &ServerNetwork::start);

    // controller
    controller->setNetwork(network);

    //  mode
    controller->setModel(mode);
    connect(mode, &ServerNetworkModel::sendJson, network, &ServerNetwork::sendJson);
    connect(mode, &ServerNetworkModel::sendJsonToBCServer, network,
            &ServerNetwork::sendJsonToBCServer);
    connect(mode, &ServerNetworkModel::broadcastToClient, network,
            &ServerNetwork::broadcastToClient);

    QTimer::singleShot(2000, mode, &ServerNetworkModel::start);
}

MainWindow::~MainWindow()
{
    thread->quit();
    QThread::msleep(1000);
    delete ui;
}
