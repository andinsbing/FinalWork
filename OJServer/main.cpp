#include "mainwindow.h"
#include <QApplication>

#include "../library/initialization/initialization.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    Initialization::initializeALL();

    MainWindow w;
    w.show();

    return a.exec();
}
