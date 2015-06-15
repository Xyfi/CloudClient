#include "clientrequestbuilder.hpp"

ClientRequestBuilder::ClientRequestBuilder()
{

}

ClientRequestBuilder::~ClientRequestBuilder()
{

}

QByteArray ClientRequestBuilder::buildAuthenticationRequest(QString username, QString password, int machineId, bool authOnly){
    QByteArray response;
    QDataStream in(&response, QIODevice::WriteOnly);
    in << (quint8)REQID_AUTHENTICATION;
    in << username;
    in << password;
    in << machineId;
    in << authOnly;
    return response;
}

QByteArray ClientRequestBuilder::buildFileUploadRequest(quint64 filesize, QString directory, QString filename){
    QByteArray response;
    QDataStream in(&response, QIODevice::WriteOnly);
    in << (quint8) REQID_FILE_UPLOAD;
    in << (quint64) filesize;
    in << directory;
    in << filename;
    return response;
}

QByteArray ClientRequestBuilder::buildFileDownloadRequest(QString directory, QString filename){
    QByteArray response;
    QDataStream in(&response, QIODevice::WriteOnly);
    in << (quint8) REQID_FILE_DOWNLOAD;
    in << directory;
    in << filename;
    return response;
}

QByteArray ClientRequestBuilder::buildFileDeletionRequest(QString directory, QString filename){
    QByteArray response;
    QDataStream in(&response, QIODevice::WriteOnly);
    in << (quint8) REQID_FILE_DELETION;
    in << directory;
    in << filename;
    return response;
}

QByteArray ClientRequestBuilder::buildFileChangesRequest(quint64 revisionNumber, int machineId){
    QByteArray response;
    QDataStream in(&response, QIODevice::WriteOnly);
    in << (quint8) REQID_FILE_CHANGES;
    in << machineId;
    in << revisionNumber;
    return response;
}

QByteArray ClientRequestBuilder::buildSuccessResponse(bool success){
    QByteArray response;
    QDataStream in(&response, QIODevice::WriteOnly);
    in << (quint8) RESPID_SUCCESS;
    in << success;
    return response;
}
