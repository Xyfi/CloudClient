#ifndef CLIENTFILEHANDLER_H
#define CLIENTFILEHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QFile>
#include <QFileInfo>

#include "clientrequestbuilder.hpp"
#include "serverresponseparser.hpp"
#include "filewatcherdatabase.hpp"
#include "messagequeue.hpp"

#define DEFAULT_TIMEOUT 10000 // 10 Seconds
#define BUFFER_SIZE 1024 * 64

#define STATUS_COMPLETE 1
#define STATUS_RETRY_LATER 2
#define STATUS_FAILED 3
#define STATUS_CONNECTION_FAILED 4

class ConnectionHandler
{
public:
    explicit ConnectionHandler();
    void setAuthenticationDetails(QString email, QString password, int machineId);
    void setServerAddress(QString ip, quint16 port);

    bool sendFile(QString);
    bool retrieveFile(QString);
    bool removeFile(QString);
    bool retrieveRemoteFileChanges(int localRevisionNumber, MessageQueue *queue);
    bool handle(Message message);

    ~ConnectionHandler();
private:
    QString username, password;
    bool authDetailsSet;
    int machineId;

    QString ip;
    quint16 port;
    bool serverAddressSet;

    bool connectToHost(QTcpSocket* socket);
    bool authenticate(QTcpSocket* socket);

    bool sendUploadRequest(QTcpSocket* socket, QString pathName);
    bool uploadFile(QTcpSocket* socket, QString pathName);

    bool sendDownloadRequest(QTcpSocket* socket, QString pathName, FileDownloadResponse* response);
    bool downloadFile(QTcpSocket* socket, QString pathNamem, FileDownloadResponse* response);

    bool sendDeletionRequest(QTcpSocket* socket, QString pathName);

    bool sendFileChangesRequest(QTcpSocket* socket, int revisionNumber, ChangedFilesResponse* response);
    bool readFileChanges(QTcpSocket* socket, ChangedFilesResponse* response, MessageQueue *queue);
};

#endif // CLIENTFILEHANDLER_H
