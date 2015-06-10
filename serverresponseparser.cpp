#include "serverresponseparser.hpp"

ServerResponseParser::ServerResponseParser()
{

}

ServerResponseParser::~ServerResponseParser()
{

}

BasicOkResponse ServerResponseParser::parseBasicOkResponse(QByteArray response){
    BasicOkResponse result;
    QDataStream out(response);
    quint8 respid;
    out >> respid;
    if(respid != SERVER_RESPONSE_ID_OK){
        result.parseSuccess = false;
    } else {
        result.parseSuccess = true;
        out >> result.ok;
        out >> result.flags;
    }
    return result;
}
AuthenticationResponse ServerResponseParser::parseAuthenticationResponse(QByteArray response){
    return parseBasicOkResponse(response);
}
UploadRequestResponse ServerResponseParser::parseUploadRequestResponse(QByteArray response){
    return parseBasicOkResponse(response);
}
UploadResponse ServerResponseParser::parseUploadResponse(QByteArray response){
    return parseBasicOkResponse(response);
}
FileDeletionResponse ServerResponseParser::parseFileDeletionResponse(QByteArray response){
    return parseBasicOkResponse(response);
}

FileDownloadResponse ServerResponseParser::parseFileDownloadResponse(QByteArray response){
    FileDownloadResponse result;
    QDataStream out(response);
    quint8 respid;
    out >> respid;
    if(respid != SERVER_RESPONSE_ID_DOWNLOAD){
        result.parseSuccess = false;
    } else {
        result.parseSuccess = true;
        out >> result.ok;
        out >> result.filesize;
    }
    return result;
}

ChangedFilesResponse ServerResponseParser::parseChangedFilesResponse(QByteArray response){
    ChangedFilesResponse result;
    QDataStream out(response);
    quint8 respid;
    out >> respid;
    if(respid != SERVER_RESPONSE_ID_CHANGED_FILES){
        result.parseSuccess = false;
    } else {
        result.parseSuccess = true;
        out >> result.ok;
        out >> result.nChanges;
        out >> result.revisionNumber;
        out >> result.flags;
    }
    return result;
}
