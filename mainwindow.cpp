#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connect(ui->signInButton, SIGNAL(clicked()), this, SLOT(loginClicked()));

    QString sEmail, sPassword;
    if (Settings::getSetting(Settings::SET_USERNAME, &sEmail) &&
        Settings::getSetting(Settings::SET_PASSWORD, &sPassword)) {
        ui->emailLineEdit->setText(sEmail);
        ui->passwordLineEdit->setText(sPassword);

        if (sEmail.isEmpty() && sPassword.isEmpty()) {
            ui->rememberCheckBox->setChecked(false);
        } else {
            ui->rememberCheckBox->setChecked(true);
        }
    } else {
        ui->emailLineEdit->setText("");
        ui->passwordLineEdit->setText("");
        ui->rememberCheckBox->setChecked(false);
    }
    qDebug() << sEmail << sPassword;
    // ui->emailLineEdit->setText("a.botteram@gmail.com");
    // ui->passwordLineEdit->setText("password1234");
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::loginClicked() {
    QString email = ui->emailLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    QString strPatt = "\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,4}\\b";
    QRegExp regex(strPatt);
    if(!regex.exactMatch(email)) {
        ui->emailLineEdit->setStyleSheet("QLineEdit { border : 2px solid red}");
        return;
    } else if (password.length() < 4) {
        ui->passwordLineEdit->setStyleSheet("QLineEdit { border : 2px solid yellow}");
        return;
    }

    bool rememberMe = ui->rememberCheckBox->isChecked();
//    if(rememberMe = ui->rememberCheckBox->isChecked()) {
//        // TODO: if statement to check if data is written successfully
//        Settings::setSetting(Settings::SET_USERNAME, ui->emailLineEdit->text());
//        Settings::setSetting(Settings::SET_PASSWORD, ui->passwordLineEdit->text());
//    } else {
//        Settings::setSetting(Settings::SET_USERNAME, "");
//        Settings::setSetting(Settings::SET_PASSWORD, "");
//    }

    emit loginRequest(email, password, rememberMe);
}

void MainWindow::on_actionAbout_triggered()
{
    About dialog;
    dialog.setModal(true);
    dialog.exec();
}
