#include "../library/file/dataaccess.h"
#include "../library/initialization/initialization.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    Initialization::initializeALL();

    MainWindow w;
    w.show();

    DataAccess da;

    //    auto d = da.importTopic(R"(F:\QtProject\FinalWork\Topic\1)");
    //    da.exportTopic(d, R"(F:\QtProject\FinalWork\Topic\2)");

    return a.exec();
}
