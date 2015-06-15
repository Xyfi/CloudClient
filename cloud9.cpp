#include "cloud9.hpp"

#include <QDebug>

Cloud9::Cloud9(QObject *parent) : QObject(parent) {
    //Init settings
    static Settings settings;
    connect(&mainWindow, SIGNAL(loginRequest(QString,QString, bool)), this, SLOT(setAuthenticationDetails(QString, QString, bool)));
    connect(this, SIGNAL(startAuthentication(QString, QString, bool)), &synchronizer, SLOT(authenticate(QString, QString, bool)));
    connect(&synchronizer, SIGNAL(authenticationSuccess(bool, QString, QString, bool)), this, SLOT(authenticationSuccess(bool, QString, QString, bool)));
    connect(this, SIGNAL(startSync()), &synchronizer, SLOT(startSync()));
    connect(&configurationWindow, SIGNAL(startSync()), &synchronizer, SLOT(startSync()));

    //Check remember login
    QString username, password;
    if(Settings::getSetting(Settings::SET_USERNAME, &username) && Settings::getSetting(Settings::SET_PASSWORD, &password)) {
//        mainWindow.close();
//        emit startSync();
        mainWindow.show();
        qDebug() << "remembered";
    } else {
        mainWindow.show();
    }

    if(!QDir("watched").exists()) {
        QDir().mkdir("watched");
    }

    synchronizer.moveToThread(&synchronizerThread);
    synchronizerThread.start();
}

Cloud9::~Cloud9() {

}

void Cloud9::setAuthenticationDetails(QString email, QString password, bool rememberMe) {
    int iMachineId;
    QString sMachineId;
    if (Settings::getSetting(Settings::SET_MACHINE_ID, &sMachineId)) {
        iMachineId = sMachineId.toInt();
        qDebug() << "exsisting machine id";
    } else {
        qsrand(time(NULL));
        iMachineId = qrand();
        Settings::setSetting(Settings::SET_MACHINE_ID, QString::number(iMachineId));
        qDebug() << "non-exsisting machine id";
    }

    synchronizer.setAuthenticationDetails(email, password, iMachineId);
    emit startAuthentication(email, password, rememberMe);
}

void Cloud9::authenticationSuccess(bool status, QString email, QString password, bool rememberMe) {
    if(status) {
        if(rememberMe) {
            Settings::setSetting(Settings::SET_USERNAME, email);
            Settings::setSetting(Settings::SET_PASSWORD, password);
        } else {
            Settings::setSetting(Settings::SET_USERNAME, "");
            Settings::setSetting(Settings::SET_PASSWORD, "");
        }
//        mainWindow.close();

        QString sFirstRun;
        Settings::getSetting(Settings::SET_FIRST_RUN, &sFirstRun);
        if (sFirstRun.toInt() == 1) {
            configurationWindow.show();
        } else {
            emit startSync();
        }
    } else {
        //show feedback login failed
    }
}
