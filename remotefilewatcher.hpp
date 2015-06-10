#ifndef REMOTEFILEWATCHER_H
#define REMOTEFILEWATCHER_H

#include "messagequeue.hpp"
#include "connectionhandler.hpp"

class RemoteFileWatcher
{
public:
    RemoteFileWatcher(MessageQueue *queue, ConnectionHandler *connection);
    void checkForChanges(int localRevisionNumber);
private:
    MessageQueue *queue;
    ConnectionHandler *connection;
};

#endif // REMOTEFILEWATCHER_H
