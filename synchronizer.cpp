#include "synchronizer.hpp"
#include <QDebug>

Synchronizer::Synchronizer(QObject* parent) : QObject(parent),
    remoteWatcher(&queue, &connection),
    queueHandler(&queue, &connection)
{
    connection.setServerAddress("192.168.1.50", 1337);
    syncFolderSet = false;
}

Synchronizer::~Synchronizer() {
    delete localWatcher;
}

void Synchronizer::setSyncFolder(QString sSyncFolderPath) {
    if (!syncFolderSet) {
        localWatcher = new LocalFileWatcher(sSyncFolderPath, &queue);
        syncFolderSet = true;
    }
}

void Synchronizer::run() {
    localWatcher->checkForChanges();
    remoteWatcher.checkForChanges(0);
    // TODO Handle merge conflicts
    queueHandler.start();
    qDebug() << "Start refresh";
    localWatcher->refreshDatabase();
}

void Synchronizer::setAuthenticationDetails(QString email, QString password, int machineId) {
    connection.setAuthenticationDetails(email, password, machineId);
}

void Synchronizer::startSync() {
    QString sSyncFolderPath;
    // TODO: if statement of te checken of de query resultaat geeft
    Settings::getSetting(Settings::SET_SYNC_FOLDER, &sSyncFolderPath);
    this->setSyncFolder(sSyncFolderPath);
    run();
}


void Synchronizer::authenticate(QString email, QString password, bool rememberMe) {
    emit authenticationSuccess(connection.authenticateOnly(), email, password, rememberMe);
}
