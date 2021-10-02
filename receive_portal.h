#ifndef RECEIVE_PORTAL_H
#define RECEIVE_PORTAL_H

#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>

namespace Ui {
class receive_portal;
}

class receive_portal : public QDialog
{
    Q_OBJECT

public:
    explicit receive_portal(QWidget *parent = nullptr);
    ~receive_portal();

public slots:
    void newConnection();
    void readSocket();

private:
    Ui::receive_portal *ui;
    QString ipAddr;
    QTcpServer *server;
    QTcpSocket *socket;
    FILE *fp;
    qint64 size;
    qint64 received;

    void setIp();
    void receiveFile();

};

#endif // RECEIVE_PORTAL_H
