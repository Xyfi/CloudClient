#ifndef FILEWATCHER_HPP
#define FILEWATCHER_HPP

#include <QObject>
#include <QFileSystemWatcher>
#include <QDebug>
#include <QDir>
#include <QDirIterator>

#include "messagequeue.hpp"
#include "filewatcherdatabase.hpp"
#include "utils.hpp"

class FileWatcherDatabase;

class LocalFileWatcher
{
private:
    MessageQueue *queue;
    FileWatcherDatabase *database;
    QDir sync_folder;
public:
    explicit LocalFileWatcher(QString sync_folder_path, MessageQueue *queue);
    bool checkForChanges();
    bool refreshDatabase();
    void checkDirectoryForChangedFiles(QString dir);
    ~LocalFileWatcher();
};

#endif // FILEWATCHER_HPP
