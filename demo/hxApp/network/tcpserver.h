#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QtNetwork/QTcpServer>
#include <hx/hx_handle.h>

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    TcpServer(QObject *parent = 0);

    QList<QTcpSocket*> tcpSocketList;
    void sendMessage(iodata i_data, unsigned int d_size);
private slots:
    void disconnectslot(int handle);
    void readMessage(QTcpSocket* tcpSocket);
signals:
    void MessageSGN(iodata,unsigned int);

    // QTcpServer interface
protected:
    void incomingConnection(qintptr handle);
};

#endif // TCPSERVER_H
