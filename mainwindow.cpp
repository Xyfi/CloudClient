#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->signInButton, SIGNAL(clicked()), this, SLOT(loginClicked()));
    ui->emailLineEdit->setText("a.botteram@gmail.com");
    ui->passwordLineEdit->setText("password1234");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loginClicked(){
    QString email = ui->emailLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    QString strPatt = "\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,4}\\b";
    QRegExp regex(strPatt);
    if(!regex.exactMatch(email)){
        ui->emailLineEdit->setStyleSheet("QLineEdit { border : 2px solid red}");
        return;
    } else if (password.length() < 4) {
        ui->passwordLineEdit->setStyleSheet("QLineEdit { border : 2px solid yellow}");
        return;
    }
    emit loginRequest(email, password);
}
