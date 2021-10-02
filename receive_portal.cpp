#include "receive_portal.h"
#include "ui_receive_portal.h"
#include <QList>
#include <QNetworkInterface>
#include <QMessageBox>
#include <QThread>
#include <stdio.h>
#include <QDir>

receive_portal::receive_portal(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::receive_portal)
{
      ui->setupUi(this);
      server = new QTcpServer(this);
      received = 0;

      setIp();

      ui->ipLabel->setText(QString("You're ip address: %1\n\n Send this to the sender.").arg(ipAddr));
      ui->sender->setText("Waiting for the sender...");

      ui->ipLabel->setAlignment(Qt::AlignCenter);
      ui->sender->setAlignment(Qt::AlignCenter);
      ui->filename->setAlignment(Qt::AlignCenter);
      ui->progress->setAlignment(Qt::AlignCenter);

      ui->progressBar->hide();

      if(!server->listen(QHostAddress::LocalHost, 6969)){
        ui->sender->setText("Failed to listen on the port 6969. Make sure that the port is not in use.");
        return;
      }

      connect(server, SIGNAL(newConnection()),this, SLOT(newConnection()));

}

receive_portal::~receive_portal()
{
    delete ui;
}

void receive_portal::setIp()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();

     for(int nIter=0; nIter<list.count(); nIter++){
          if(!list[nIter].isLoopback()){
              if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol ){
                  ipAddr = list[nIter].toString();
                  break;
              }
          }
     }
}

void receive_portal::newConnection()
{
    socket = server->nextPendingConnection();
    socket->waitForReadyRead();

    QString client = socket->readAll().trimmed();
    QString ipClient = socket->peerAddress().toString();

    QString text = QString("New connection request received from username:%1 ip:%2 \nDo you want to accept it?").arg(client,ipClient);

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "New connection",text ,QMessageBox::Yes|QMessageBox::No);

      if (reply == QMessageBox::No) {
        socket->write("Refused");
        socket->close();
        return;
      }

    socket->write("Accepted");
    ui->sender->setText(QString("Connected to %1 from %2").arg(client,ipClient));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));

}

void receive_portal::readSocket()
{
    QString msg = socket->readAll();
    QStringList info = msg.split("||");
    QString text = QString("The sender wants to send you filename:%1 size:%2 \n Do you want to accept it?").arg(info[0],info[1]);


    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "New file send request",text ,QMessageBox::Yes|QMessageBox::No);

      if (reply == QMessageBox::No) {
        socket->write("Refused");
        return;
      }

    socket->write("Accepted");
    size = info[1].toLongLong();

    QDir dir = QDir::home().filePath(QString("Qt_fileShare/%1").arg(info[0]));
    char *path = dir.absolutePath().toLocal8Bit().data();

    fp = std::fopen(path,"w");

    ui->filename->setText(QString("Receiving file %1 size %2").arg(info[0],info[1]));
    ui->progressBar->show();
    ui->progressBar->setRange(0,size);

    disconnect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
    receiveFile();
}

void receive_portal::receiveFile()
{

    while(socket->waitForReadyRead(3000)){
        int toRead = ((size-received)>=1024) ? 1024:size-received;
        QByteArray data = socket->read(toRead);

        if(toRead == 0){
            socket->readAll();
            ui->progress->setText("Transfer Completed");
            ui->progressBar->hide();
            ui->filename->setText("");

            connect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
            std::fclose(fp);
            received = 0;

            return;
        }

        int n = std::fwrite(data,sizeof(char),data.length(),fp);
        std::fflush(fp);

        received+=n;

        ui->progress->setText(QString("Received %1").arg(received));
        ui->progressBar->setValue(received);

        socket->write("OK",2);
    }

}
