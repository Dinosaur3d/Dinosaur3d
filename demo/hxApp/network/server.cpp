#include "server.h"
#include "ui_server.h"
#include <QMessageBox>

#include <hx/socket/header.h>
#include <hx/socket/handle_dem.h>
#include <hx/socket/handle_cameraimaging.h>
#include <hx/socket/handle_latlongitude.h>
#include <hx/socket/handle_sundirection.h>
#include <hx/socket/handle_user_cameraimaging.h>
#include <hx/socket/handle_vehiclesimulation.h>

server::server(QWidget *parent) :
    QWidget(parent),
    port(17000),
    is_open(false),
    ui(new Ui::server)
{
    ui->setupUi(this);
    m_tcpserver=new TcpServer;
    connect(m_tcpserver,SIGNAL(MessageSGN(iodata,unsigned int)),this,SLOT(readMessage(iodata,unsigned int)));
    connect(m_tcpserver,SIGNAL(acceptError(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
    ui->startServerBtn->setEnabled(true);
    ui->closeServerBtn->setEnabled(false);
}

server::~server()
{
    delete ui;
}

void server::on_portBtn_clicked()
{
    if(ui->portEdit->text().isEmpty())
    {
        QMessageBox::information(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("端口不能为空！"));
        return;
    }
    port=ui->portEdit->text().toInt();
    if(port<0)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("端口不能为负数！\n请重新设置"));
        port=17000;
        return;
    }
    port=ui->portEdit->text().toInt();
    qDebug()<<"Port is set!";
    qDebug()<<port;
}

void server::on_startServerBtn_clicked()
{
    if(!is_open)
    {
        m_tcpserver->listen(QHostAddress::Any,port);
        is_open=true;
        ui->startServerBtn->setEnabled(false);
        ui->closeServerBtn->setEnabled(true);
        qDebug()<<"Server open!";
        qDebug()<<port;
    }
}

void server::on_closeServerBtn_clicked()
{
    m_tcpserver->close();
    is_open=false;
    ui->startServerBtn->setEnabled(true);
    ui->closeServerBtn->setEnabled(false);
//    delete[]m_tcpserver;
//    m_tcpserver=nullptr;
    qDebug()<<"NetWork closed!";
}

void server::readMessage(iodata i_data, unsigned int size)
{
    SocketCommandHeader sch;
    sch.read(i_data);
    if(sch.commandword==handle_dem::DemReqCommand)
    {
        handle_dem h_dem;
        iodata response_idata=h_dem.handle(i_data);
        if(m_tcpserver==nullptr)
            return;
        m_tcpserver->sendMessage(response_idata,response_idata.size);
    }
    else if (sch.commandword==handle_cameraimaging::CameraImagingReqCommand)
    {
        handle_cameraimaging::SocketCameraImagingReqCommand my_scImgreq;
        my_scImgreq.read(i_data);
        if(my_scImgreq.camera_type == 7)
        {
//            iodata new_idata(i_data);
            i_data.setValue<char>(sch.getDataSize()+5,1);
            handle_cameraimaging h_came_img_a;
            iodata response_idata_a=h_came_img_a.handle(i_data);
            if(m_tcpserver==nullptr)
                return;
            m_tcpserver->sendMessage(response_idata_a,response_idata_a.size);
            i_data.setValue<char>(sch.getDataSize()+5,2);
            handle_cameraimaging h_came_img_b;
            iodata response_idata_b=h_came_img_b.handle(i_data);
            if(m_tcpserver==nullptr)
                return;
            m_tcpserver->sendMessage(response_idata_b,response_idata_b.size);
        }
        else if (my_scImgreq.camera_type == 8)
        {
            i_data.setValue<char>(sch.getDataSize()+5,3);
            handle_cameraimaging h_came_img_a;
            iodata response_idata_a=h_came_img_a.handle(i_data);
            if(m_tcpserver==nullptr)
                return;
            m_tcpserver->sendMessage(response_idata_a,response_idata_a.size);
            i_data.setValue<char>(sch.getDataSize()+5,4);
            handle_cameraimaging h_came_img_b;
            iodata response_idata_b=h_came_img_b.handle(i_data);
            if(m_tcpserver==nullptr)
                return;
            m_tcpserver->sendMessage(response_idata_b,response_idata_b.size);
        }
        else if (my_scImgreq.camera_type == 9)
        {
            i_data.setValue<char>(sch.getDataSize()+5,5);
            handle_cameraimaging h_came_img_a;
            iodata response_idata_a=h_came_img_a.handle(i_data);
            if(m_tcpserver==nullptr)
                return;
            m_tcpserver->sendMessage(response_idata_a,response_idata_a.size);
            i_data.setValue<char>(sch.getDataSize()+5,6);
            handle_cameraimaging h_came_img_b;
            iodata response_idata_b=h_came_img_b.handle(i_data);
            if(m_tcpserver==nullptr)
                return;
            m_tcpserver->sendMessage(response_idata_b,response_idata_b.size);
        }
        else
        {
            handle_cameraimaging h_came_img;
            iodata response_idata=h_came_img.handle(i_data);
            if(m_tcpserver==nullptr)
                return;
            m_tcpserver->sendMessage(response_idata,response_idata.size);
        }
    }
    else if (sch.commandword==handle_user_cameraimaging::UserCameraImagingReqCommand)
    {
        handle_user_cameraimaging h_user_came_img;
        iodata response_idata=h_user_came_img.handle(i_data);
        if(m_tcpserver==nullptr)
            return;
        m_tcpserver->sendMessage(response_idata,response_idata.size);
    }
    else if (sch.commandword==handle_vehiclesimulation::VehiclesimulationReqCommand)
    {
        //接收后刷新火星车位态，无反馈
        handle_vehiclesimulation h_veh_sim;
        iodata response_idata=h_veh_sim.handle(i_data);
//        if(m_tcpserver==nullptr)
//            return;
//        m_tcpserver->sendMessage(response_idata,response_idata.size);
    }
    else if (sch.commandword==handle_latlongitude::GetLatLongitudeReqCommond)
    {
        handle_latlongitude h_lat_lon;
        iodata response_idata=h_lat_lon.handle(i_data);
        if(m_tcpserver==nullptr)
            return;
        m_tcpserver->sendMessage(response_idata,response_idata.size);
    }
    else if (sch.commandword==handle_sundirection::SetSunDirectionCommond)
    {
        //接收后刷新光照，无反馈
        handle_sundirection h_sundir;
        iodata response_idata=h_sundir.handle(i_data);
//        if(m_tcpserver==nullptr)
//            return;
//        m_tcpserver->sendMessage(response_idata,response_idata.size);
    }
}

void server::sendMessage()
{

}

void server::displayError(QAbstractSocket::SocketError)
{
    qDebug()<<m_tcpserver->errorString();
}
