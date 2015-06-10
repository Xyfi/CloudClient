#ifndef CLOUD9_H
#define CLOUD9_H

#include <QObject>
#include <QDir>
#include <QThread>

#include "mainwindow.hpp"
#include "synchronizer.hpp"

class Cloud9 : public QObject
{
    Q_OBJECT
public:
    explicit Cloud9(QObject *parent = 0);
    ~Cloud9();
private:
    MainWindow mainWindow;
    Synchronizer synchronizer;
    QThread synchronizerThread;
signals:
    void startSync();

public slots:
    void setAuthenticationDetails(QString email, QString password);
};

#endif // CLOUD9_H
