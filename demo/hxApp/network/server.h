#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include "tcpserver.h"
#include <hx/hx_handle.h>

namespace Ui {
class server;
}

class server : public QWidget
{
    Q_OBJECT

public:
    explicit server(QWidget *parent = 0);
    ~server();

private slots:
    //设置端口
    void on_portBtn_clicked();

    //开启服务
    void on_startServerBtn_clicked();

    //关闭服务
    void on_closeServerBtn_clicked();

    void readMessage(iodata i_data, unsigned int size);
    void sendMessage();

private slots:
    void displayError(QAbstractSocket::SocketError);

private:
    Ui::server *ui;
    TcpServer *m_tcpserver;
    int port;
    bool is_open;
};

#endif // SERVER_H
