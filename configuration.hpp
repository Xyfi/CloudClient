#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <QMainWindow>
#include <QFileDialog>
#include "about.hpp"
#include "settings.hpp"

namespace Ui {
class Configuration;
}

class Configuration : public QMainWindow
{
    Q_OBJECT

public:
    explicit Configuration(QWidget *parent = 0);
    ~Configuration();

signals:
    void startSync();

private slots:
    void on_closeButton_clicked();

    void on_okButton_clicked();

    void on_browseButton_clicked();

    void on_actionAbout_triggered();

private:
    Ui::Configuration *ui;
};

#endif // CONFIGURATION_HPP
