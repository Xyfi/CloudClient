#ifndef CLIENTREQUESTBUILDER_H
#define CLIENTREQUESTBUILDER_H

#include <QByteArray>
#include <QDataStream>

#define REQID_AUTHENTICATION    (quint8)0x01
#define REQID_FILE_UPLOAD       (quint8)0x02
#define REQID_FILE_DOWNLOAD     (quint8)0x03
#define REQID_FILE_DELETION     (quint8)0x04
#define REQID_FILE_CHANGES      (quint8)0x05

#define RESPID_SUCCESS          (quint8)0xF1

class ClientRequestBuilder
{
public:
    ClientRequestBuilder();
    ~ClientRequestBuilder();

    static QByteArray buildAuthenticationRequest(QString username, QString password, int machineId);
    static QByteArray buildFileUploadRequest(quint64 filesize, QString directory, QString filename);
    static QByteArray buildFileDownloadRequest(QString directory, QString filename);
    static QByteArray buildFileDeletionRequest(QString directory, QString filename);
    static QByteArray buildFileChangesRequest(quint64 revisionNumber, int machineId);

    static QByteArray buildSuccessResponse(bool success);
};

#endif // CLIENTREQUESTBUILDER_H
