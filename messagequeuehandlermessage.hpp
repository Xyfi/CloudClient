#ifndef MESSAGEQUEUEHANDLERMESSAGE_HPP
#define MESSAGEQUEUEHANDLERMESSAGE_HPP

#include <QObject>

typedef struct {
    int type;
    QString directory;
    QString filename;
} Message;

Q_DECLARE_METATYPE(Message)

#define MESSAGE_TYPE_UPLOAD 0
#define MESSAGE_TYPE_DOWNLOAD 1
#define MESSAGE_TYPE_DELETE 2
#define MESSAGE_TYPE_DELETE_LOCALLY 3

#endif // MESSAGEQUEUEHANDLERMESSAGE_HPP

