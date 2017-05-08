#include "tcpsocket.h"


TcpSocket::TcpSocket(QObject *parent):
    QTcpSocket(parent)
{
    connect(this,SIGNAL(readyRead()),this,SLOT(emitreadyRead()));
}

void TcpSocket::emitreadyRead()
{
    emit readyReadSocket((QTcpSocket*)this);
}

