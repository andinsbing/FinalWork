#include "mainwindow.h"
#include "judger.h"
#include "ojservernetwork.h"
#include "ojservernetworkcontroller.h"
#include "ojservernetworkmodel.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QProcess>
#include <QThread>
#include <QTimer>

const QString testCode = "\n\
#include<iostream>\n\
using std::cout;\n\
using std::cin;\n\
int main()\n\
{\n\
    int a,b;\n\
    cin>>a>>b;\n\
    std::cout<<a-b;\n\
}";

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), network(new OJServerNetwork),
      networkThread(new QThread(this)), controller(new OJServerNetworkController(this)),
      model(new OJServerNetworkModel(this))
{
    ui->setupUi(this);

    // network
    network->start();
    network->moveToThread(networkThread);
    networkThread->start();

    // controller
    controller->setNetwork(network);
    controller->setModel(model);

    // mode
    connect(model, &OJServerNetworkModel::sendJson, network, &OJServerNetwork::sendJson);
    connect(model, &OJServerNetworkModel::sendJsonToBCServer, network,
            &OJServerNetwork::sendJsonToBCServer);
    QTimer::singleShot(1000, model, &OJServerNetworkModel::start);

    //    Judger      judger;
    //    QJsonObject task;
    //    task.insert("code", testCode);
    //    task.insert("time_limit", 1000);
    //    task.insert("memory_limit", 100);
    //    task.insert("language", "CPP");
    //    task.insert("index", 1);
    //    task.insert("id", 1);

    //    qDebug() << judger.judge(task);
}

MainWindow::~MainWindow()
{
    networkThread->quit();

    delete ui;
}
