#include "cloud9.hpp"

Cloud9::Cloud9(QObject *parent) : QObject(parent)
{
    //Init settings
    static Settings settings;
    connect(&mainWindow, SIGNAL(loginRequest(QString,QString)), this, SLOT(setAuthenticationDetails(QString, QString)));
    connect(this, SIGNAL(startAuthentication()), &synchronizer, SLOT(authenticate()));
    connect(&synchronizer, SIGNAL(authenticationSuccess(bool)), this, SLOT(authenticationSuccess(bool)));
    connect(this, SIGNAL(startSync()), &synchronizer, SLOT(startSync()));
    connect(&configurationWindow, SIGNAL(startSync()), &synchronizer, SLOT(startSync()));
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
    emit startAuthentication();
}

void Cloud9::authenticationSuccess(bool status) {
    if(status) {
        mainWindow.close();

        QString sFirstRun;
        Settings::getSetting(Settings::SET_FIRST_RUN, &sFirstRun);
        if (sFirstRun.toInt() == 1) {
            configurationWindow.show();
            // BUG: code after show get's executed before
            //      configurationWindow closes. emit should be executed
            //      in the "else". and the ok of configurationWindow
            //      should emit the same signal.
            //      FIXED !!?
        } else {
            emit startSync();
        }
    } else {
        //show feedback login failed
    }
}
