#include "settings.hpp"

QSqlDatabase Settings::database;

Settings::Settings(QObject *parent) : QObject(parent),
    SET_FIRST_RUN( "FirstRun" ),
    SET_SYNC_FOLDER( "SyncFolder" ),
    SET_USERNAME( "Username" ),
    SET_PASSWORD( "Password" )
{
    QSqlDatabase::addDatabase(DB_TYPE, "Settings");
    database = QSqlDatabase::database("Settings");
    database.setDatabaseName(DB_NAME);
    database.open();

    database.exec("pragma foreign_keys = on");

    if(!database.tables().contains("Settings")){
        QSqlQuery query(database);
        bool s = query.exec(TABLE_SETTINGS);
        if(!s){
            qDebug() << "Query failed";
        }
        setSetting(SET_FIRST_RUN, "1");
        qDebug() << "Table Settings added";
    }
    if(!database.open()){
        qDebug() << "Opening new database failed!";
    }
}

Settings::~Settings()
{
    database.close();
}

bool Settings::setSetting(QString key, QString value){
    QString queryString = "INSERT OR REPLACE INTO Settings(key, value) VALUES(:key, :value)";
    QSqlQuery query(database);
    if(!query.prepare(queryString)){
        qDebug() << "[Settings::setSetting] Prepare failed:" << query.lastError().text();
        return false;
    }
    query.bindValue(":key", key);
    query.bindValue(":value", value);
    if(!query.exec()){
        qDebug() << "[Settings::setSetting] Exec failed:" << query.lastError().text();
        return false;
    }
    return true;
}

bool Settings::getSetting(QString key, QString *value){
    QString queryString = "SELECT value FROM Settings WHERE key = :key";
    QSqlQuery query(database);
    if(!query.prepare(queryString)){
        qDebug() << "[Settings::getSetting] Prepare failed:" << query.lastError().text();
        return false;
    }
    query.bindValue(":key", key);
    if(!query.exec()){
        qDebug() << "[Settings::setSetting] Exec failed:" << query.lastError().text();
        return false;
    }
    if(!query.next()){
        qDebug() << "[Settings::setSetting] Next failed:" << query.lastError().text();
        return false;
    }
    *value = query.value(0).toString();
    return true;
}
