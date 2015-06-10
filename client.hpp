#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>

#include "messagequeuehandler.hpp"
#include "localfilewatcher.hpp"
#include "connectionhandler.hpp"
#include "messagequeue.hpp"
#include "remotefilewatcher.hpp"

class Client : public QObject
{
    Q_OBJECT
public:
    Client(QObject* parent = 0);
    void run();
protected:

private:
    MessageQueue queue;
    ConnectionHandler connection;
    LocalFileWatcher localWatcher;
    RemoteFileWatcher remoteWatcher;
    MessageQueueHandler queueHandler;


};

#endif // CLIENT_H
