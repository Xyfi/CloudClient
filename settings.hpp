#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QtSql>
#include <QDebug>

#define DB_NAME "ClientDatabase.db"
#define DB_TYPE "QSQLITE"

#define TABLE_SETTINGS "CREATE TABLE Settings("\
    "key TEXT PRIMARY KEY UNIQUE NOT NULL,"\
    "value TEXT NOT NULL"\
")"

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = 0);
    ~Settings();

    static bool setSetting(QString key, QString value);
    static bool getSetting(QString key, QString *value);

    static const QString SET_FIRST_RUN;
    static const QString SET_SYNC_FOLDER;
    static const QString SET_USERNAME;
    static const QString SET_PASSWORD;
    static const QString SET_MACHINE_ID;

private:
    static QSqlDatabase database;

signals:

public slots:
};

#endif // SETTINGS_H
