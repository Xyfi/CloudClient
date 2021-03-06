#ifndef CLOUD9_H
#define CLOUD9_H

#include <QObject>
#include <QDir>
#include <QThread>

#include <time.h>

#include "mainwindow.hpp"
#include "synchronizer.hpp"
#include "configuration.hpp"
#include "about.hpp"
#include "settings.hpp"

class Cloud9 : public QObject
{
    Q_OBJECT
public:
    explicit Cloud9(QObject *parent = 0);
    ~Cloud9();
private:
    MainWindow mainWindow;
    Configuration configurationWindow;
    About aboutWindow;
    QThread synchronizerThread;
    Synchronizer synchronizer;
signals:
    void startSync();
    void startAuthentication(QString email, QString password, bool rememberMe);

public slots:
    void setAuthenticationDetails(QString email, QString password, bool rememberMe);
    void authenticationSuccess(bool status, QString email, QString password, bool rememberMe);
};

#endif // CLOUD9_H
