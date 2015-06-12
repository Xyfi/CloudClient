#include "configuration.hpp"
#include "ui_configuration.h"
#include <QDebug>

Configuration::Configuration(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Configuration)
{
    ui->setupUi(this);
}

Configuration::~Configuration()
{
    delete ui;
}

void Configuration::on_closeButton_clicked()
{
    this->close();
}

void Configuration::on_okButton_clicked()
{
    qDebug() << ui->pathEdit->text();
    // FIXME: check for legit path in ui->pathEdit
    Settings::setSetting(Settings::SET_SYNC_FOLDER, ui->pathEdit->text());
    Settings::setSetting(Settings::SET_FIRST_RUN, "0");
    this->close();
}

void Configuration::on_browseButton_clicked()
{
    ui->pathEdit->setText(QFileDialog::getExistingDirectory(this,
                                                            tr("Open Directory"),
                                                            QDir::homePath(),
                                                            QFileDialog::ShowDirsOnly |
                                                            QFileDialog::DontResolveSymlinks));
}

void Configuration::on_actionAbout_triggered()
{
    About aDialog;
    aDialog.setModal(true);
    aDialog.exec();
}
