#ifndef DATABASEHANDLER_HPP
#define DATABASEHANDLER_HPP

#include <QObject>
#include <QtSql>
#include <QDebug>
#include <QDirIterator>
#include <QList>

#include "utils.hpp"

#define DB_NAME "ClientDatabase.db"
#define DB_TYPE "QSQLITE"

#define TABLE_DIR       	"CREATE TABLE Directories (path TEXT NOT NULL PRIMARY KEY UNIQUE)"
#define TABLE_NAME_DIR      "Directories"

#define TABLE_FILE          "CREATE TABLE Files (directory TEXT NOT NULL, name TEXT NOT NULL, hash TEXT NOT NULL, FOREIGN KEY(directory) REFERENCES Directories(path) ON DELETE CASCADE ON UPDATE CASCADE, PRIMARY KEY(directory, name))"
#define TABLE_NAME_FILE     "Files"

#define SYNC_FOLDER "watched"

class FileWatcherDatabase
{
public:
    explicit FileWatcherDatabase();
    ~FileWatcherDatabase();

    void addAllFilesAndDirectories(QString directory);
    void deleteAllFilesAndDirectories();

    QStringList getAllDirectories();
    QStringList getFilesInDirectory(QString directory);
    QString getFileHash(QString filepath);

private:
    QSqlDatabase database;
    void addAllFilesInDirectory(QString directory);

};

#endif // DATABASEHANDLER_HPP
