#include "client.hpp"

Client::Client(QObject* parent) : QObject(parent),
    localWatcher("watched", &queue),
    remoteWatcher(&queue, &connection),
    queueHandler(&queue, &connection)
{
    connection.setServerAddress("192.168.1.50", 1337);
    connection.setAuthenticationDetails("a.botteram@gmail.com", "password", 1337);
}

void Client::run(){
    localWatcher.checkForChanges();
    remoteWatcher.checkForChanges(0);
    // TODO Handle merge conflicts
    queueHandler.start();
    localWatcher.refreshDatabase();
}
