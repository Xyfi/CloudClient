#include "localfilewatcher.hpp"

LocalFileWatcher::LocalFileWatcher(QString sync_folder_path, MessageQueue *queue) :
    queue(queue)
{
    database = new FileWatcherDatabase;
    sync_folder = QDir(sync_folder_path);
    if(!sync_folder.exists()){
        qDebug() << "Error! Folder passed into FileWatcher not valid!";
        throw QException();
    }
}

LocalFileWatcher::~LocalFileWatcher()
{
    delete queue;
    delete database;
}

bool LocalFileWatcher::checkForChanges(){
    QDirIterator existingFolderIterator(sync_folder.dirName(),
                                        QDir::AllDirs | QDir::NoDotAndDotDot,
                                        QDirIterator::Subdirectories);
    QStringList storedirectoryList = database->getAllDirectories();
    checkDirectoryForChangedFiles(sync_folder.dirName());
    while ( existingFolderIterator.hasNext() ) {
        QString dir = existingFolderIterator.next();
        if(storedirectoryList.contains(dir)){
            storedirectoryList.removeOne(dir);
        } else {
            //emit directoryAdded(dir);
        }
        checkDirectoryForChangedFiles(dir);
    }
    storedirectoryList.removeOne(sync_folder.path());
    for(QString string : storedirectoryList){
        //emit directoryRemoved(string);
    }
    refreshDatabase();
    return true;
}

void LocalFileWatcher::checkDirectoryForChangedFiles(QString dir){
    QDirIterator existingFileIterator(dir, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
    QStringList storedFileList = database->getFilesInDirectory(dir);
    while(existingFileIterator.hasNext()){
        QFileInfo file = QFileInfo(existingFileIterator.next());
        if(storedFileList.contains(file.filePath())){
            QString db_hash = database->getFileHash(file.filePath());
            QString local_hash = Utils::generateHash(file.filePath());
            if(QString::compare(db_hash, local_hash) == 0){
            } else {
                qDebug() << db_hash << local_hash;
                queue->addMessage({MESSAGE_TYPE_UPLOAD, file.path(), file.fileName()});
                //emit fileUpdated(file.filePath());
            }
            storedFileList.removeOne(file.filePath());
        } else {
            queue->addMessage({MESSAGE_TYPE_UPLOAD, file.path(), file.fileName()});
            storedFileList.removeOne(file.filePath());
            //emit fileAdded(file.filePath());
        }
    }
    for(QString string : storedFileList){
        QFileInfo file = QFileInfo(string);
        queue->addMessage({MESSAGE_TYPE_DELETE, file.path(), file.fileName()});
        //emit fileRemoved(string);
    }
}

bool LocalFileWatcher::refreshDatabase(){
    database->deleteAllFilesAndDirectories();
    database->addAllFilesAndDirectories(sync_folder.dirName());
    return true;
}
