#include "synchronizer.hpp"
#include <QDebug>

Synchronizer::Synchronizer(QObject* parent) : QObject(parent),
    localWatcher("watched", &queue),
    remoteWatcher(&queue, &connection),
    queueHandler(&queue, &connection)
{
    connection.setServerAddress("192.168.1.50", 1337);
}

void Synchronizer::run() {
    localWatcher.checkForChanges();
    remoteWatcher.checkForChanges(0);
    // TODO Handle merge conflicts
    queueHandler.start();
    localWatcher.refreshDatabase();
}

void Synchronizer::setAuthenticationDetails(QString email, QString password, int machineId) {
    connection.setAuthenticationDetails(email, password, machineId);
}

void Synchronizer::startSync() {
    run();
}


void Synchronizer::authenticate(QString email, QString password, bool rememberMe) {
    emit authenticationSuccess(connection.authenticateOnly(), email, password, rememberMe);
}
