#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
class MainWindow;
}

class ClientNetwork;
class ClientNetworkController;
class ClientViewController;
class ClientModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void test();

private:
    Ui::MainWindow*          ui;
    ClientNetwork*           network;
    ClientNetworkController* networkController;
    ClientViewController*    viewController;
    ClientModel*             model;
    class LoginDialog*       loginDialog;
    class SubmitDialog*      submitDialog;
    QThread*                 thread;
    class InfoDialog*        infoDialog;
    class TaskDialog*        taskDialog;
};

#endif  // MAINWINDOW_H
