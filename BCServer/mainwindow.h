#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow*                  ui;
    class BCServerNetwork*           network;
    class QThread*                   networkThread;
    class BCServerNetworkController* netowrkController;
    class BCServerNetworkModel*      networkModel;
    class SQLController*             sqlController;
    class QThread*                   sqlThread;
    class TaskManager*               manager;
};

#endif  // MAINWINDOW_H
