#ifndef SERVERMESSAGEPARSER_H
#define SERVERMESSAGEPARSER_H

#include <QtGlobal>
#include <QDataStream>

#define SERVER_RESPONSE_ID_OK (quint8)0x01
#define SERVER_RESPONSE_ID_DOWNLOAD (quint8)0x02
#define SERVER_RESPONSE_ID_CHANGED_FILES (quint8)0x03

typedef struct {
    bool parseSuccess;
    bool ok;
    qint8 flags;
} BasicOkResponse;

typedef BasicOkResponse AuthenticationResponse;
typedef BasicOkResponse UploadRequestResponse;
typedef BasicOkResponse UploadResponse;
typedef BasicOkResponse FileDeletionResponse;

typedef struct {
    bool parseSuccess;
    bool ok;
    quint64 filesize;
} FileDownloadResponse;

typedef struct {
    bool parseSuccess;
    bool ok;
    int nChanges;
    quint64 revisionNumber;
    qint8 flags;
} ChangedFilesResponse;

class ServerResponseParser
{
public:
    ServerResponseParser();
    ~ServerResponseParser();

    static ChangedFilesResponse parseChangedFilesResponse(QByteArray response);
    static FileDeletionResponse parseFileDeletionResponse(QByteArray response);
    static FileDownloadResponse parseFileDownloadResponse(QByteArray response);
    static AuthenticationResponse parseAuthenticationResponse(QByteArray response);
    static UploadRequestResponse parseUploadRequestResponse(QByteArray response);
    static UploadResponse parseUploadResponse(QByteArray response);

private:
    static BasicOkResponse parseBasicOkResponse(QByteArray response);


};

#endif // SERVERMESSAGEPARSER_H
