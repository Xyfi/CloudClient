#-------------------------------------------------
#
# Project created by QtCreator 2015-06-10T11:50:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Cloud9-Client-Presentatie
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    client.cpp \
    clientrequestbuilder.cpp \
    connectionhandler.cpp \
    filewatcherdatabase.cpp \
    localfilewatcher.cpp \
    messagequeue.cpp \
    messagequeuehandler.cpp \
    remotefilewatcher.cpp \
    serverresponseparser.cpp \
    synchronizer.cpp \
    utils.cpp

HEADERS  += \
    client.hpp \
    clientrequestbuilder.hpp \
    connectionhandler.hpp \
    filewatcherdatabase.hpp \
    localfilewatcher.hpp \
    messagequeue.hpp \
    messagequeuehandler.hpp \
    messagequeuehandlermessage.hpp \
    remotefilewatcher.hpp \
    serverresponseparser.hpp \
    synchronizer.hpp \
    utils.hpp \
    mainwindow.hpp

FORMS    += mainwindow.ui
