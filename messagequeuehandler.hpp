#ifndef MESSAGEQUEUEHANDLER_H
#define MESSAGEQUEUEHANDLER_H

#include <QObject>
#include <QSemaphore>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>

#include "filewatcherdatabase.hpp"
#include "connectionhandler.hpp"
#include "messagequeue.hpp"
#include "messagequeuehandlermessage.hpp"

class MessageQueueHandler : public QObject
{
    Q_OBJECT
public:
    explicit MessageQueueHandler(MessageQueue *queue, ConnectionHandler *connection, QObject *parent = 0);
    ~MessageQueueHandler();
    void start();

private:
    MessageQueue *queue;
    ConnectionHandler *connection;

};

#endif // MESSAGEQUEUEHANDLER_H
