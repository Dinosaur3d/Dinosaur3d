#include "tcpserver.h"
#include <QDataStream>

#include "tcpsocket.h"

TcpServer::TcpServer(QObject *parent):
    QTcpServer(parent)
{

}

void TcpServer::sendMessage(iodata i_data, unsigned int d_size)
{
    for (int i=0;i<tcpSocketList.count();i++)
    {
        QTcpSocket *item=tcpSocketList.at(i);
//        QByteArray block;
//        QDataStream out(&block,QIODevice::WriteOnly);
//        out.setVersion(QDataStream::Qt_5_4);
//        out<<c_data;
        item->write(i_data.data,d_size);
        if(item->waitForBytesWritten())
        {
            qDebug()<<"server success write";
        }else {
            qDebug()<<"server fail write";
        }
    }
}

void TcpServer::disconnectslot(int handle)
{
    for (int i=0;i<tcpSocketList.count();i++)
    {
        QTcpSocket *item=tcpSocketList.at(i);
        if(item->socketDescriptor()== handle)
        {
            tcpSocketList.removeAt(i);
            return;
        }
    }
}

void TcpServer::readMessage(QTcpSocket *tcpSocket)
{
//    QString string;
    QByteArray block = tcpSocket->readAll();
//    QDataStream in(block);
//    in.setVersion(QDataStream::Qt_5_4);
//    in>>string;
//    qDebug()<<string;
    char *c_data=block.data();
    iodata i_data;
    i_data.set(block.size(),c_data);
    emit MessageSGN(i_data,block.size());
}

void TcpServer::incomingConnection(qintptr handle)
{
    TcpSocket *tcpSocket = new TcpSocket(this);
    tcpSocket->setSocketDescriptor(handle);
    connect(tcpSocket,SIGNAL(readyReadSocket(QTcpSocket*)),this,SLOT(readMessage(QTcpSocket*)));
    connect(tcpSocket,SIGNAL(disconnected(int)),this,SLOT(disconnectslot(int)));
    tcpSocketList.append(tcpSocket);
}

