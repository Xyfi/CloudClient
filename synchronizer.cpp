#include "synchronizer.hpp"
#include <QDebug>

Synchronizer::Synchronizer(QObject* parent) : QObject(parent),
    remoteWatcher(&queue, &connection),
    queueHandler(&queue, &connection)
{
    connection.setServerAddress("192.168.1.50", 1337);
}

Synchronizer::~Synchronizer() {
    delete localWatcher;
}

void Synchronizer::setSyncFolder(QString sSyncFolderPath) {
    localWatcher = new LocalFileWatcher(sSyncFolderPath, &queue);
}

void Synchronizer::run(){
    localWatcher->checkForChanges();
    remoteWatcher.checkForChanges(0);
    // TODO Handle merge conflicts
    queueHandler.start();
    qDebug() << "Start refresh";
    localWatcher->refreshDatabase();
}

void Synchronizer::setAuthenticationDetails(QString email, QString password, int machineId){
    connection.setAuthenticationDetails(email, password, machineId);
}

void Synchronizer::startSync(){
    QString sSyncFolderPath;
    // TODO: if statement of te checken of de query resultaat geeft
    Settings::getSetting(Settings::SET_SYNC_FOLDER, &sSyncFolderPath);
    this->setSyncFolder(sSyncFolderPath);
    run();
}


void Synchronizer::authenticate(){
    emit authenticationSuccess(connection.authenticateOnly());
}
