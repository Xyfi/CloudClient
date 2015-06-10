#include "remotefilewatcher.hpp"

RemoteFileWatcher::RemoteFileWatcher(MessageQueue *queue, ConnectionHandler *connection):
    queue(queue), connection(connection)
{

}

void RemoteFileWatcher::checkForChanges(int localRevisionNumber){
    connection->retrieveRemoteFileChanges(localRevisionNumber, queue);
}

