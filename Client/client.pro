#-------------------------------------------------
#
# Project created by QtCreator 2019-02-26T23:12:52
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS CLIENT

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    ../library/file/filesystem.cpp \
    ../library/global_config/globalconfig.cpp \
    ../library/initialization/initialization.cpp \
    ../library/log/textlog.cpp \
    ../library/network/network.cpp \
    ../library/network/session.cpp \
    ../library/network/tcpserver.cpp \
    ../library/network/tcpsocket.cpp \
    clientnetwork.cpp \
    ../library/network/synchronizationtask.cpp \
    clientviewcontroller.cpp \
    clientnetworkcontroller.cpp \
    ../library/data_structure/ojdatastructure.cpp\
    logindialog.cpp \
    submitdialog.cpp \
    ../library/network/networkcontroller.cpp \
    ../library/network/networkreceiver.cpp \
    ../library/network/networksender.cpp \
    ../library/network/networkmodel.cpp \
    infodialog.cpp \
    clientmodel.cpp \
    ../library/json/jsonfunction.cpp \
    taskdialog.cpp

HEADERS += \
        mainwindow.h \
    ../library/data_structure/doublemap.h \
    ../library/file/filesystem.h \
    ../library/global_config/globalconfig.h \
    ../library/initialization/initialization.h \
    ../library/log/textlog.h \
    ../library/network/network.h \
    ../library/network/protocol.h \
    ../library/network/session.h \
    ../library/network/tcpserver.h \
    ../library/network/tcpsocket.h \
    clientnetwork.h \
    ../library/network/synchronizationtask.h \
    clientviewcontroller.h \
    clientnetworkcontroller.h \
    ../library/data_structure/ojdatastructure.h\
    logindialog.h \
    submitdialog.h \
    ../library/network/networkcontroller.h \
    ../library/network/networkreceiver.h \
    ../library/network/networksender.h \
    ../library/network/networkmodel.h \
    infodialog.h \
    clientmodel.h \
    ../library/json/jsonfunction.h \
    taskdialog.h

FORMS += \
        mainwindow.ui \
    logindialog.ui \
    submitdialog.ui \
    infodialog.ui \
    taskdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
