#include "../library/global_config/globalconfig.h"
#include "../library/initialization/initialization.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    Initialization::initializeALL();
    QApplication a(argc, argv);
    MainWindow   w;
    w.show();

    return a.exec();
}
