#include "cloud9.hpp"

Cloud9::Cloud9(QObject *parent) : QObject(parent)
{
    //Init settings
    static Settings settings;
    connect(&mainWindow, SIGNAL(loginRequest(QString,QString)), this, SLOT(setAuthenticationDetails(QString, QString)));
    connect(this, SIGNAL(startSync()), &synchronizer, SLOT(startSync()));
    mainWindow.show();

    if(!QDir("watched").exists()){
        QDir().mkdir("watched");
    }

    Settings::setSetting("TestSetting", "TRUE");
    QString testSetting;
    Settings::getSetting("TestSetting", &testSetting);
    qDebug() << testSetting;

    synchronizer.moveToThread(&synchronizerThread);
    synchronizerThread.start();
}

Cloud9::~Cloud9()
{

}

void Cloud9::setAuthenticationDetails(QString email, QString password){
    synchronizer.setAuthenticationDetails(email, password, 1);
    emit startSync();
}
