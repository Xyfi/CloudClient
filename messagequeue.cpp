#include "messagequeue.hpp"

MessageQueue::MessageQueue(QObject *parent) : QObject(parent)
{
    QSqlDatabase::addDatabase(DB_TYPE, "MessageQueue");
    database = QSqlDatabase::database("MessageQueue");
    database.setDatabaseName(DB_NAME);
    database.open();
    if(!database.open()){
        qDebug() << "Opening database failed!";
        throw QException();
    }
    database.exec("pragma foreign_keys = on");
    if(!database.tables().contains("Messages")){
        QSqlQuery query(database);
        if(!query.exec(TABLE_MSG)){
            qDebug() << "Query failed" << query.lastError().text();
            throw QException();
        }
    }
}

MessageQueue::~MessageQueue()
{

}

bool MessageQueue::addMessage(Message message)
{
    qDebug() <<  "Adding" << message.filename;
    QString queryString = "INSERT INTO Messages(type, directory, filename) VALUES(:type, :directory, :filename)";
    QSqlQuery query(database);
    if(!query.prepare(queryString)){
        qDebug() << "[MessageQueue::addMessage] Error on prepare:" << query.lastError().text();
        return false;
    }
    query.bindValue(":type", message.type);
    query.bindValue(":directory", message.directory);
    query.bindValue(":filename", message.filename);
    if(!query.exec()){
        qDebug() << "[MessageQueue::addMessage] Error on exec:" << query.lastError().text();
        return false;
    }
    query.clear();
    emit newMessage();
    return true;
}

bool MessageQueue::getUnhandledMessage(Message *message)
{
    QString queryString = "SELECT type, directory, filename FROM Messages WHERE beingHandled = 0";
    QSqlQuery query(database);
    if(!query.prepare(queryString)){
        qDebug() << "[MessageQueue::getUnhandledMessage] Error on prepare:" << query.lastError().text();
        return false;
    }
    if(!query.exec()){
        qDebug() << "[MessageQueue::getUnhandledMessage] Error on exec:" << query.lastError().text();
        return false;
    }
    if(!query.next()){
        qDebug() << "[MessageQueue::getUnhandledMessage] Error on next:" << query.lastError().text();
        return false;
    }
    message->type = query.value(0).toInt();
    message->directory = query.value(1).toString();
    message->filename = query.value(2).toString();
    if(!setMessageBeingHandled(*message, true)){
        qDebug() << "[MessageQueue::getUnhandledMessage] Error on setMessageBeingHandled:" << query.lastError().text();
        return false;
    }
    return true;
}

bool MessageQueue::messageHandled(Message message, bool success)
{
    if(!success){
        if(setMessageBeingHandled(message, false)){
            emit newMessage();
            return true;
        }
        return false;
    } else {
        return removeMessage(message);
    }
}

// PRIVATE METHODS

bool MessageQueue::setMessageBeingHandled(Message message, bool beingHandled)
{
    QString queryString = "UPDATE Messages SET beingHandled = :beingHandled WHERE directory = :directory AND filename = :filename AND type = :type";
    QSqlQuery query(database);
    if(!query.prepare(queryString)){
        qDebug() << "[MessageQueue::setMessageBeingHandled] Error on prepare:" << query.lastError().text();
        return false;
    }
    query.bindValue(":beingHandled", beingHandled);
    query.bindValue(":directory", message.directory);
    query.bindValue(":filename", message.filename);
    query.bindValue(":type", message.type);
    if(!query.exec()){
        qDebug() << "[MessageQueue::setMessageBeingHandled] Error on exec:" << query.lastError().text();
        return false;
    }
    return true;
}

bool MessageQueue::removeMessage(Message message)
{
    QString queryString = "DELETE FROM Messages WHERE directory = :directory AND filename = :filename AND type = :type";
    QSqlQuery query(database);
    if(!query.prepare(queryString)){
        qDebug() << "[MessageQueue::removeMessage] Error on prepare:" << query.lastError().text();
        return false;
    }
    query.bindValue(":directory", message.directory);
    query.bindValue(":filename", message.filename);
    query.bindValue(":type", message.type);
    if(!query.exec()){
        qDebug() << "[MessageQueue::removeMessage] Error on exec:" << query.lastError().text();
        return false;
    }
    return true;
}

