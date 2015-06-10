#include "connectionhandler.hpp"

ConnectionHandler::ConnectionHandler()
{
    authDetailsSet = false;
    serverAddressSet = false;
}

ConnectionHandler::~ConnectionHandler()
{

}

void ConnectionHandler::setAuthenticationDetails(QString email, QString password, int machineId)
{
    this->username = email;
    this->password = password;
    this->machineId = machineId;
    authDetailsSet = true;
}

void ConnectionHandler::setServerAddress(QString ip, quint16 port)
{
    this->ip = ip;
    this->port = port;
    serverAddressSet = true;
}

bool ConnectionHandler::sendFile(QString pathName)
{
    if(!serverAddressSet){
        qDebug() << "Server address not set. Please call setServerAddress().";
        return false;
    }
    if(!authDetailsSet){
        qDebug() << "Authentication details not set. Please call setAuthenticationDetails().";
        return false;
    }
    QTcpSocket* socket = new QTcpSocket();
    if(!connectToHost(socket)){
        delete socket;
        return false;
    }
    if(!authenticate(socket)){
        delete socket;
        return false;
    }
    if(!sendUploadRequest(socket, pathName)){
        delete socket;
        return false;
    }
    qDebug() << "Starting upload";
    if(!uploadFile(socket, pathName)){
        delete socket;
        return false;
    }
    qDebug() << "[ClientFileHandler::sendFile] File upload succesfull";
    delete socket;
    return true;
}

bool ConnectionHandler::retrieveFile(QString pathName){
    if(!serverAddressSet){
        qDebug() << "Server address not set. Please call setServerAddress().";
        return false;
    }
    if(!authDetailsSet){
        qDebug() << "Authentication details not set. Please call setAuthenticationDetails().";
        return false;
    }
    QTcpSocket* socket = new QTcpSocket();
    if(!connectToHost(socket)){
        delete socket;
        return false;
    }
    if(!authenticate(socket)){
        delete socket;
        return false;
    }
    FileDownloadResponse response;
    if(!sendDownloadRequest(socket, pathName, &response)){
        delete socket;
        return false;
    }
    if(!downloadFile(socket, pathName, &response)){;
        delete socket;
        return false;
    }
    qDebug() << "[ClientFileHandler::retrieveFile] File download succesfull";
    delete socket;
    return true;
}

bool ConnectionHandler::removeFile(QString pathName){
    if(!serverAddressSet){
        qDebug() << "Server address not set. Please call setServerAddress().";
        return false;
    }
    if(!authDetailsSet){
        qDebug() << "Authentication details not set. Please call setAuthenticationDetails().";
        return false;
    }
    QTcpSocket* socket = new QTcpSocket();
    if(!connectToHost(socket)){
        delete socket;
        return false;
    }
    if(!authenticate(socket)){
        delete socket;
        return false;
    }
    if(!sendDeletionRequest(socket, pathName)){
        delete socket;
        return false;
    }
    qDebug() << "[ClientFileHandler::removeFile] File deletion succesfull";
    delete socket;
    return true;
}

bool ConnectionHandler::retrieveRemoteFileChanges(int localRevisionNumber, MessageQueue *queue){
    if(!serverAddressSet){
        qDebug() << "Server address not set. Please call setServerAddress().";
        return false;
    }
    if(!authDetailsSet){
        qDebug() << "Authentication details not set. Please call setAuthenticationDetails().";
        return false;
    }
    QTcpSocket* socket = new QTcpSocket();
    if(!connectToHost(socket)){
        delete socket;
        return false;
    }
    if(!authenticate(socket)){
        delete socket;
        return false;
    }
    ChangedFilesResponse response;
    if(!sendFileChangesRequest(socket, localRevisionNumber, &response)){
        delete socket;
        return false;
    }
    if(!readFileChanges(socket, &response, queue)){
        delete socket;
        return false;
    }
    qDebug() << "[ClientFileHandler::retrieveRemoteFileChanges] Retrieving file changes succesfull";
    delete socket;
    return true;
}

bool ConnectionHandler::handle(Message message){
    QString filepath = message.directory + "/" + message.filename;
    switch(message.type){
    case MESSAGE_TYPE_UPLOAD:
        if(sendFile(filepath)){
            return true;
        } else {
            return false;
        }
        break;
    case MESSAGE_TYPE_DOWNLOAD:
        if(retrieveFile(filepath)){
            return true;
        } else {
            return false;
        }
        break;
    case MESSAGE_TYPE_DELETE:
        if(removeFile(filepath)){
            return true;
        } else {
            return false;
        }
        break;
    default:
        return false;
        break;
    }
    return false;
}

// PRIVATE

bool ConnectionHandler::connectToHost(QTcpSocket* socket){
    socket->connectToHost(ip, port);
    return socket->waitForConnected(DEFAULT_TIMEOUT);
}

bool ConnectionHandler::authenticate(QTcpSocket *socket){
    socket->write(ClientRequestBuilder::buildAuthenticationRequest(username,password,machineId));
    if(!socket->waitForBytesWritten(DEFAULT_TIMEOUT)){
        qDebug() << "[Authentication] Timeout while waiting for bytes to be written to socket. Connection closed.";
        socket->close();
        return false;
    }
    if(!socket->waitForReadyRead(DEFAULT_TIMEOUT)){
        qDebug() << "[Authentication] Timeout while waiting for server response. Connection closed.";
        socket->close();
        return false;
    }
    AuthenticationResponse response =
            ServerResponseParser::parseAuthenticationResponse(socket->readAll());
    if(!response.parseSuccess || !response.ok){
        qDebug() << "[Authentication] Authentication failed.";
        return false;
    }
    return true;
}

bool ConnectionHandler::sendUploadRequest(QTcpSocket *socket, QString pathName){

    QFile file(pathName);
    QFileInfo info(file);
    if(!file.exists()){
        qDebug() << "[sendUploadRequest] Non-existent file. Connection closed." << pathName;
        socket->close();
        return false;
    }

    socket->write(ClientRequestBuilder::buildFileUploadRequest(file.size(), info.path(), info.fileName()));

    if(!socket->waitForBytesWritten(DEFAULT_TIMEOUT)){
        qDebug() << "[sendUploadRequest] Timeout while waiting for bytes to be written to socket. Connection closed.";
        qDebug() << socket->errorString();
        socket->close();
        return false;
    }
    if(!socket->waitForReadyRead(DEFAULT_TIMEOUT)){
        qDebug() << "[sendUploadRequest] Timeout while waiting for server response. Connection closed.";
        socket->close();
        return false;
    }
    UploadRequestResponse response =
            ServerResponseParser::parseUploadRequestResponse(socket->readAll());
    if(!response.parseSuccess || !response.ok){
        qDebug() << "[sendUploadRequest] Parsing failed or server send NOT OK Response.";
        return false;
    }
    return true;
}

bool ConnectionHandler::uploadFile(QTcpSocket *socket, QString pathName){
    QFile file(pathName);
    if(!file.open(QIODevice::ReadOnly)){
        return false;
    }
    char data[BUFFER_SIZE];
    quint64 len;
    quint64 total = 0;
    while((len = file.read(data, BUFFER_SIZE)) > 0){
        socket->write(data, len);
        total += len;
        if(!socket->waitForBytesWritten(DEFAULT_TIMEOUT)){
            qDebug() << "[ClientFileHandler::uploadFile] Error while waitingForBytesWritten. Connection closed.";
            socket->close();
            file.close();
            return false;
        }
    }
    file.close();
    if(!socket->waitForReadyRead(DEFAULT_TIMEOUT)){
        qDebug() << "[uploadFile] Timeout while waiting for server response. Connection closed.";
        socket->close();
        return false;
    }
    UploadResponse response =
            ServerResponseParser::parseUploadResponse(socket->readAll());
    if(!response.parseSuccess || !response.ok){
        qDebug() << "[uploadFile] Parsing failed or server send NOT OK Response.";
        return false;
    }
    return true;
}

bool ConnectionHandler::sendDownloadRequest(QTcpSocket *socket, QString pathName, FileDownloadResponse* response){

    QFile file(pathName);
    QFileInfo info(file);

    socket->write(ClientRequestBuilder::buildFileDownloadRequest(info.path(), info.fileName()));

    if(!socket->waitForBytesWritten(DEFAULT_TIMEOUT)){
        qDebug() << "[sendDownloadRequest] Timeout while waiting for bytes to be written to socket. Connection closed.";
        socket->close();
        return false;
    }
    if(!socket->waitForReadyRead(DEFAULT_TIMEOUT)){
        qDebug() << "[sendDownloadRequest] Timeout while waiting for server response. Connection closed.";
        socket->close();
        return false;
    }
    *response =
            ServerResponseParser::parseFileDownloadResponse(socket->readAll());
    if(!response->parseSuccess || !response->ok){
        qDebug() << "[sendDownloadRequest] Parsing failed or server send NOT OK Response.";
        socket->close();
        return false;
    }
    return true;
}

bool ConnectionHandler::downloadFile(QTcpSocket *socket, QString pathName, FileDownloadResponse* response){
    QFile file(pathName);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    quint64 receivedTotal = 0;
    quint64 received;
    char data[BUFFER_SIZE];
    while(receivedTotal < response->filesize){
        if(!socket->bytesAvailable()){
            if(!socket->waitForReadyRead(DEFAULT_TIMEOUT)){
                qDebug() << "[ClientFileHandler::downloadFile] Error while waitForReadyRead. Connection closed.";
                socket->close();
                file.close();
                return false;
            }
        }
        received = socket->read(data, BUFFER_SIZE);
        receivedTotal += received;
        file.write(data, received);
    }
    file.close();
    return true;
}

bool ConnectionHandler::sendDeletionRequest(QTcpSocket *socket, QString pathName){
    QFile file(pathName);
    QFileInfo info(file);

    socket->write(ClientRequestBuilder::buildFileDeletionRequest(info.path(), info.fileName()));

    if(!socket->waitForBytesWritten(DEFAULT_TIMEOUT)){
        qDebug() << "[sendDeletionRequest] Timeout while waiting for bytes to be written to socket. Connection closed.";
        socket->close();
        return false;
    }
    if(!socket->waitForReadyRead(DEFAULT_TIMEOUT)){
        qDebug() << "[sendDeletionRequest] Timeout while waiting for server response. Connection closed.";
        socket->close();
        return false;
    }
    FileDeletionResponse response =
            ServerResponseParser::parseFileDeletionResponse(socket->readAll());
    if(!response.parseSuccess || !response.ok){
        qDebug() << "[sendDeletionRequest] Parsing failed or server send NOT OK Response.";
        socket->close();
        return false;
    }
    return true;
}

bool ConnectionHandler::sendFileChangesRequest(QTcpSocket *socket, int revisionNumber, ChangedFilesResponse* response){

    socket->write(ClientRequestBuilder::buildFileChangesRequest(revisionNumber, machineId));

    if(!socket->waitForBytesWritten(DEFAULT_TIMEOUT)){
        qDebug() << "[sendFileChangesRequest] Timeout while waiting for bytes to be written to socket. Connection closed.";
        socket->close();
        return false;
    }
    if(!socket->waitForReadyRead(DEFAULT_TIMEOUT)){
        qDebug() << "[sendFileChangesRequest] Timeout while waiting for server response. Connection closed.";
        socket->close();
        return false;
    }
    *response = ServerResponseParser::parseChangedFilesResponse(socket->readAll());
    if(!response->parseSuccess || !response->ok){
        qDebug() << "[sendFileChangesRequest] Parsing server response failed. Connection closed.";
        socket->close();
        return false;
    }
    return true;
}

bool ConnectionHandler::readFileChanges(QTcpSocket *socket, ChangedFilesResponse* response, MessageQueue *queue){
    socket->waitForReadyRead(DEFAULT_TIMEOUT);
    QByteArray data = socket->readAll();
    QDataStream out(data);
    QString dir, name;
    bool deleted;
    for(int i = 0 ; i < response->nChanges ; i++ ){
        out >> dir >> name >> deleted;
        if(deleted){
            if(!queue->addMessage({MESSAGE_TYPE_DELETE_LOCALLY, dir, name})){
                qDebug() << "[Adding]ConnectionHandler::readFileChanges] Adding delete message failed.";
            }
        } else {
            if(!queue->addMessage({MESSAGE_TYPE_DOWNLOAD, dir, name})){
                qDebug() << "[Adding]ConnectionHandler::readFileChanges] Adding upload message failed.";
            }
        }
    }
    return true;
}


