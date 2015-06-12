#-------------------------------------------------
#
# Project created by QtCreator 2015-06-10T11:50:53
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += C++11

TARGET = Cloud9-Client-Presentatie
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    clientrequestbuilder.cpp \
    connectionhandler.cpp \
    filewatcherdatabase.cpp \
    localfilewatcher.cpp \
    messagequeue.cpp \
    messagequeuehandler.cpp \
    remotefilewatcher.cpp \
    serverresponseparser.cpp \
    utils.cpp \
    synchronizer.cpp \
    cloud9.cpp \
    about.cpp \
    configuration.cpp \
    settings.cpp

HEADERS  += \
    clientrequestbuilder.hpp \
    connectionhandler.hpp \
    filewatcherdatabase.hpp \
    localfilewatcher.hpp \
    messagequeue.hpp \
    messagequeuehandler.hpp \
    messagequeuehandlermessage.hpp \
    remotefilewatcher.hpp \
    serverresponseparser.hpp \
    utils.hpp \
    mainwindow.hpp \
    synchronizer.hpp \
    cloud9.hpp \
    about.hpp \
    configuration.hpp \
    settings.hpp

FORMS    += mainwindow.ui \
            configuration.ui \
            about.ui

DISTFILES += \
    images/icon.png

RESOURCES += \
    resources.qrc
