#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include <QObject>
#include <QFile>
#include <QtSql>
#include <QMutex>
#include <QMutexLocker>

#include "messagequeuehandlermessage.hpp"

#define DB_NAME "ClientDatabase.db"
#define DB_TYPE "QSQLITE"

#define TABLE_MSG           "CREATE TABLE Messages (type INTEGER NOT NULL, beingHandled INTEGER NOT NULL DEFAULT 0, directory TEXT NOT NULL, filename TEXT NOT NULL, PRIMARY KEY(type, directory, filename))"
#define TABLE_NAME_MSG      "Messages"

class MessageQueue : public QObject
{
    Q_OBJECT
public:
    explicit MessageQueue(QObject *parent = 0);

    bool addMessage(Message message);
    bool getUnhandledMessage(Message *message);

    bool messageHandled(Message message, bool success);

    ~MessageQueue();
private:
    QSqlDatabase database;
    QMutex addMessageMutex;
    QMutex getMessageMutex;

    bool setMessageBeingHandled(Message message, bool beingHandled);
    bool removeMessage(Message message);

signals:
    void newMessage();

public slots:
};

#endif // MESSAGEQUEUE_H
