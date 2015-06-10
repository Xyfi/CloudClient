#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>

#include "messagequeuehandler.hpp"
#include "localfilewatcher.hpp"
#include "connectionhandler.hpp"
#include "messagequeue.hpp"
#include "remotefilewatcher.hpp"

class Synchronizer : public QObject
{
    Q_OBJECT
public:
    Synchronizer(QObject* parent = 0);
    void setAuthenticationDetails(QString email, QString password, int machineId);

protected:

private:
    MessageQueue queue;
    ConnectionHandler connection;
    LocalFileWatcher localWatcher;
    RemoteFileWatcher remoteWatcher;
    MessageQueueHandler queueHandler;
    void run();

public slots:
    void startSync();
};

#endif // CLIENT_H
