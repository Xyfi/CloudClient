#ifndef CLIENTFILEHANDLER_H
#define CLIENTFILEHANDLER_H

#include <QObject>
#include <QSslSocket>
#include <QFile>
#include <QFileInfo>
#include <Qlist>
#include <QSslError>

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

class ConnectionHandler : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionHandler(QObject* parent = 0);
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

    QSslSocket* createSocket();

    bool connectToHost(QSslSocket* socket);
    bool authenticate(QSslSocket* socket);

    bool sendUploadRequest(QSslSocket* socket, QString pathName);
    bool uploadFile(QSslSocket* socket, QString pathName);

    bool sendDownloadRequest(QSslSocket* socket, QString pathName, FileDownloadResponse* response);
    bool downloadFile(QSslSocket* socket, QString pathNamem, FileDownloadResponse* response);

    bool sendDeletionRequest(QSslSocket* socket, QString pathName);

    bool sendFileChangesRequest(QSslSocket* socket, int revisionNumber, ChangedFilesResponse* response);
    bool readFileChanges(QSslSocket* socket, ChangedFilesResponse* response, MessageQueue *queue);

private slots:
    void handleErrors(const QList<QSslError> & errors);
    void encryptedConnection();
};

#endif // CLIENTFILEHANDLER_H
