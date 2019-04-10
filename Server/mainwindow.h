#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QJsonObject>
#include <QMainWindow>
class Network;
class QThread;

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow*                ui;
    class ServerNetwork*           network;
    QThread*                       thread;
    class ServerNetworkController* controller;
    class ServerNetworkModel*      mode;
};

#endif  // MAINWINDOW_H
