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
        if (!connection->handle(message)) {
            queue->messageHandled(message, false);
            qDebug() << "[MessageQueueHandler::start] Handling message failed";
            break;
        } else {
            queue->messageHandled(message, true);
        }
    }

}
