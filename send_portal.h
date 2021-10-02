#ifndef SEND_PORTAL_H
#define SEND_PORTAL_H

#include <QDialog>
#include <QTcpSocket>


namespace Ui {
class send_portal;
}

class send_portal : public QDialog
{
    Q_OBJECT

public:
    explicit send_portal(QWidget *parent = nullptr, QString name="");
    ~send_portal();

private slots:
    void on_pushButton_clicked();

private:
    Ui::send_portal *ui;
    QString ipaddr;
    QTcpSocket *socket;
    QByteArray username;
    char *filename;
    FILE *fp;
    qint64 size;
    qint64 sent;

    void getIpAddr();
    void sendFile();
    bool sendMetaData();
};

#endif // SEND_PORTAL_H
