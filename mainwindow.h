#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "receive_portal.h"
#include "send_portal.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_receive_clicked();

    void on_send_clicked();

private:
    Ui::MainWindow *ui;
    QString username;
    receive_portal *recv_portal;
    send_portal *snd_portal;

    void getUsername();


};
#endif // MAINWINDOW_H
