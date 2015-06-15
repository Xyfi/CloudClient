#include "messagequeuehandler.hpp"

MessageQueueHandler::MessageQueueHandler(MessageQueue *queue, ConnectionHandler *connection, QObject *parent) : QObject(parent),
    queue(queue), connection(connection)
{
}

MessageQueueHandler::~MessageQueueHandler()
{
    delete queue;
}

void MessageQueueHandler::start()
{
    Message message;
    while(queue->getUnhandledMessage(&message)) {
        qDebug() << "getUnhandledMessage";
        if (message.type == MESSAGE_TYPE_DELETE_LOCALLY) {
            QString syncFolder;
            if (!Settings::getSetting(Settings::SET_SYNC_FOLDER, &syncFolder)) {
                queue->messageHandled(message, false);
                qDebug() << "[MessageQueueHandler::start] Retriving setting failed.";
            }
            QString fileName = Utils::convertRelativePathToAbsolutePath(syncFolder, message.directory + "/" + message.filename);
            QFile file(fileName);
            if (file.exists()) {
                if (!file.remove()) {
                    queue->messageHandled(message, false);
                    qDebug() << "[MessageQueueHandler::start] Removing file failed.";
                }
            }
        } else if (!connection->handle(message)) {
            queue->messageHandled(message, false);
            qDebug() << "[MessageQueueHandler::start] Handling message failed";
            break;
        } else {
            queue->messageHandled(message, true);
        }
    }

}
