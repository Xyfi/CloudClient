#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>

#include "messagequeuehandler.hpp"
#include "localfilewatcher.hpp"
#include "connectionhandler.hpp"
#include "messagequeue.hpp"
#include "remotefilewatcher.hpp"
#include "settings.hpp"

class Synchronizer : public QObject
{
    Q_OBJECT
public:
    Synchronizer(QObject* parent = 0);
    ~Synchronizer();
    void setSyncFolder(QString sSyncFolderPath);
    void setAuthenticationDetails(QString email, QString password, int machineId);

protected:

private:
    MessageQueue queue;
    ConnectionHandler connection;
    LocalFileWatcher *localWatcher;
    RemoteFileWatcher remoteWatcher;
    MessageQueueHandler queueHandler;
    void run();

public slots:
    void startSync();
    void authenticate(QString email, QString password, bool rememberMe);

signals:
    void authenticationSuccess(bool success, QString email, QString password, bool rememberMe);
};

#endif // CLIENT_H
