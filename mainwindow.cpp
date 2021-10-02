#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QDebug>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    getUsername();

    QDir dir = QDir::home().filePath("Qt_fileShare/");
    if(!dir.exists()){
        dir.mkpath(".");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getUsername()
{
    username = QInputDialog::getText(this,tr("Enter Username"), tr("Username"), QLineEdit::Normal);
    if(username.length()==0){
        exit(1);
    }
}

void MainWindow::on_receive_clicked()
{
    recv_portal = new receive_portal(this);
    hide();
    recv_portal->show();

}

void MainWindow::on_send_clicked()
{
    snd_portal = new send_portal(this,username);
    hide();
    snd_portal->show();
}
