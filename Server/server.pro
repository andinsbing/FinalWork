#-------------------------------------------------
#
# Project created by QtCreator 2018-01-31T21:30:34
#
#-------------------------------------------------

QT       += core gui  network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Server
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS SERVER

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

FORMS += \
    mainwindow.ui

HEADERS += \
    mainwindow.h \
    mysqlconnector.h \
    networkconfig.h \
    ../library/network/protocol.h \
    ../library/network/tcpserver.h \
    ../library/network/tcpsocket.h \
    ../library/log/textlog.h \
    ../library/initialization/initialization.h \
    ../library/network/session.h \
    ../library/network/network.h \
    ../library/data_structure/doublemap.h \
    ../library/file/filesystem.h \
    ../library/global_config/globalconfig.h \
    servernetwork.h \
    servernetworkcontroller.h \
    ../library/network/networkcontroller.h \
    ../library/network/networkmodel.h \
    ../library/network/networkreceiver.h \
    servernetworkmodel.h \
    ../library/network/networkidfunction.h \
    serversession.h \
    ../library/json/jsonfunction.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mysqlconnector.cpp \
    networkconfig.cpp \
    ../library/network/tcpserver.cpp \
    ../library/network/tcpsocket.cpp \
    ../library/log/textlog.cpp \
    ../library/initialization/initialization.cpp \
    ../library/network/session.cpp \
    ../library/network/network.cpp \
    ../library/file/filesystem.cpp \
    ../library/global_config/globalconfig.cpp \
    servernetwork.cpp \
    servernetworkcontroller.cpp \
    ../library/network/networkcontroller.cpp \
    ../library/network/networkmodel.cpp \
    ../library/network/networkreceiver.cpp \
    servernetworkmodel.cpp \
    ../library/network/networkidfunction.cpp \
    serversession.cpp \
    ../library/json/jsonfunction.cpp
