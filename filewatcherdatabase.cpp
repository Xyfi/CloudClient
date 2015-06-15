#include "filewatcherdatabase.hpp"

FileWatcherDatabase::FileWatcherDatabase() {
    QSqlDatabase::addDatabase(DB_TYPE, "DatabaseHandler");
    database = QSqlDatabase::database("DatabaseHandler");
    database.setDatabaseName(DB_NAME);
    database.open();

    database.exec("pragma foreign_keys = on");

    if(!database.tables().contains("Directories")) {
        QSqlQuery query(database);
        bool s = query.exec(TABLE_DIR);
        if(!s) {
            qDebug() << "Query failed";
        }
        qDebug() << "Table Directories added";
    }
    if(!database.tables().contains("Files")) {
        QSqlQuery query(database);
        bool s = query.exec(TABLE_FILE);
        if(!s) {
            qDebug() << "Query failed" << database.lastError();
        }
        qDebug() << "Table Files added";
    }
    if(!database.open()) {
        qDebug() << "Opening new database failed!";
    }
}

FileWatcherDatabase::~FileWatcherDatabase()
{
    database.close();
}

void FileWatcherDatabase::addAllFilesAndDirectories(QString rootDirectory) {
    QSqlQuery query(database);
    QString queryString = QString("INSERT INTO Directories (path) VALUES('%1')").arg(rootDirectory);
    bool s = query.exec(queryString);
    if(!s) {
        qDebug() << "Adding root-directory failed" << queryString;
        qDebug() << "Error: " << database.lastError().text();
    }
    addAllFilesInDirectory(rootDirectory);
    QDirIterator it(rootDirectory, QDir::AllDirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while(it.hasNext()) {
        QSqlQuery query(database);
        QString directory = it.next();
        QString queryString = QString("INSERT INTO Directories (path) VALUES('%1')").arg(directory);
        bool s = query.exec(queryString);
        if(!s) {
            qDebug() << "Adding directory failed" << queryString;
            qDebug() << "Error: " << database.lastError().text();
        }
        addAllFilesInDirectory(directory);
    }
}

void FileWatcherDatabase::addAllFilesInDirectory(QString directory) {
    QDirIterator fileIt(directory, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
    while(fileIt.hasNext()) {
        QSqlQuery query(database);
        QFileInfo fileinfo = QFileInfo(fileIt.next());
        QString directory = fileinfo.path();
        QString filename = fileinfo.fileName();
        QString hash = QString(Utils::generateHash(fileinfo.filePath()));
        QString queryString = "INSERT OR REPLACE INTO Files (directory, name, hash) VALUES(:directory,:filename,:hash)";
        if(!query.prepare(queryString)) {
            qDebug() << "[DatabaseHandler::addAllFilesInDirectory] Prepare failed:" << query.lastError().text();
            return;
        }
        query.bindValue(":directory", directory);
        query.bindValue(":filename", filename);
        query.bindValue(":hash", hash);
        if(!query.exec()) {
            qDebug() << "Adding file failed" << queryString;
            qDebug() << "Error: " << query.lastError().text();
        }
        query.clear();
    }
}

void FileWatcherDatabase::deleteAllFilesAndDirectories() {
    QString queryString = QString("Delete from Directories");
    QSqlQuery query(database);
    bool s = query.exec(queryString);
    if(!s) {
        qDebug() << "Deleting files failed" << queryString;
    }
}

QStringList FileWatcherDatabase::getAllDirectories() {

    QStringList list;
    QString queryString = ("SELECT * FROM Directories");
    QSqlQuery query(database);
    if(!query.exec(queryString)) {
        qDebug() << "Retreiving directories failed" << queryString;
        qDebug() << "Error: " << database.lastError();
    }
    QString dir;
    while(query.next()) {
        dir = query.value(0).toString();
        list << dir;
    }
    return list;
}

QStringList FileWatcherDatabase::getFilesInDirectory(QString directory) {
    QStringList list;
    QString queryString = ("SELECT * FROM Files WHERE directory=:directory");
    QSqlQuery query = QSqlQuery(database);
    if(!query.prepare(queryString)) {
        qDebug() << "Error on prepare";
        return list;
    }
    query.bindValue(":directory", directory);
    if(!query.exec()) {
        qDebug() << "Retreiving files in " << directory << " failed" << queryString;
        qDebug() << "Error: " << database.lastError().text();
    }
    while(query.next()) {
        QString filepath = query.value(0).toString() + "/" + query.value(1).toString();
        list.append( filepath );
    }
    return list;
}

QString FileWatcherDatabase::getFileHash(QString filepath) {
    QString queryString = "SELECT (hash) FROM Files WHERE directory=:directory AND name=:name";
    QSqlQuery query(database);
    if(!query.prepare(queryString)) {
        qDebug() << "Error on prepare";
        return "";
    }
    QFileInfo fileinfo(filepath);
    query.bindValue(":directory", fileinfo.path());
    query.bindValue(":name", fileinfo.fileName());
    if(!query.exec()) {
        qDebug() << "Retreiving file-hash of " << fileinfo.filePath() << " failed" << queryString;
        qDebug() << "Error: " << database.lastError().text();
    }
    if(query.next()) {
        return query.value(0).toString();
    }

    return "";
}

