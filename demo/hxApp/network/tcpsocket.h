#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QtNetwork/QTcpSocket>

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    TcpSocket(QObject *parent = 0);
signals:
    void disconnected(int);
    void readyReadSocket(QTcpSocket*);
public slots:
    void emitreadyRead();
};

#endif // TCPSOCKET_H
