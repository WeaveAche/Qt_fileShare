#include "send_portal.h"
#include "ui_send_portal.h"

#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <stdio.h>

send_portal::send_portal(QWidget *parent, QString name) :
    QDialog(parent),
    ui(new Ui::send_portal)
{
    ui->setupUi(this);

    username.append(name);
    socket = new QTcpSocket(this);
    sent = 0;

    getIpAddr();
    socket->connectToHost(ipaddr,6969);


    if(socket->waitForConnected(3)){
        socket->write(username);

        if(socket->waitForReadyRead()){
            QString res = socket->readAll();
            if(res == "Refused"){
                QMessageBox::warning(this,"Failed","The receiver rejected your connection request.");
                exit(1);
            }
        }else{
            QMessageBox::warning(this,"Failed","The receiver didnt accept your connection request.");
            exit(1);
        }
    }else{
        QMessageBox::warning(this,"Failed","Connection to the provided ip failed. Make sure that the receiver has an open connection.");
        exit(1);
    }


    ui->receiver->setAlignment(Qt::AlignCenter);
    ui->file->setAlignment(Qt::AlignCenter);
    ui->progress->setAlignment(Qt::AlignCenter);

    ui->receiver->setText("Connected with "+ipaddr);

    ui->progressBar->hide();

}

send_portal::~send_portal()
{
    delete ui;
}

void send_portal::getIpAddr()
{
    ipaddr = QInputDialog::getText(this,tr("IP"), tr("Enter The Receiver's IP address"), QLineEdit::Normal);
    if(ipaddr.length()==0){
        exit(1);
    }
}


void send_portal::on_pushButton_clicked()
{
    filename = (char *)QFileDialog::getOpenFileName(this,"Choose a file",QDir::homePath()).toLocal8Bit().data();
    fp = std::fopen(filename,"r");

    if(sendMetaData()){
        sendFile();
    }
}

bool send_portal::sendMetaData()
{
    QFile temp(filename);
    size = temp.size();

    QByteArray msg;
    msg.append(temp.fileName().section("/",-1,-1)).append("||").append(QString("%1").arg(size));
    socket->write(msg);

    if(socket->waitForReadyRead()){
        QString res = socket->readAll();
        if(res == "Refused"){
            QMessageBox::warning(this,"Failed","The receiver rejected the send file request.");
            std::fclose(fp);
            return false;
        }
    }else{
        QMessageBox::warning(this,"Failed","The receiver didnt accept your send file request.");
        std::fclose(fp);
        return false;
    }

    return true;
}

void send_portal::sendFile()
{
    ui->pushButton->hide();
    ui->progressBar->setRange(0,size);
    ui->progressBar->show();

    ui->file->setText(QString("Sending file %1").arg(filename));

    int n;
    char buffer[1024] = {0};
    while(1){
        n = std::fread(buffer,sizeof(char),1024,fp);

        if(n==0){
            socket->write("END");
            ui->progress->setText("Transfer completed");
            std::fclose(fp);
            ui->progressBar->hide();
            ui->pushButton->show();
            ui->file->setText("");

            sent=0;
            return;
        }

        n = socket->write(buffer,n);
        memset(buffer,'\0',1024);

        sent+=n;
        ui->progress->setText(QString("Sent %1").arg(sent));
        ui->progressBar->setValue(sent);

        if(!socket->waitForReadyRead(3000)){
            return;
        }else{
            socket->readAll();
        }
    }
}
