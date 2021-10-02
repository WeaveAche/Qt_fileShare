#ifndef SENDER_H
#define SENDER_H

#include <QObject>

class sender : public QObject
{
    Q_OBJECT
public:
    explicit sender(QObject *parent = nullptr);

signals:

};

#endif // SENDER_H
