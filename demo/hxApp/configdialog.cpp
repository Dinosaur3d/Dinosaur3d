#include "configdialog.h"
#include "ui_configdialog.h"
#include <osg/Matrix>
#include <fstream>
#include <hx/hx_options.h>
#include <hx/hx_che.h>
#include <QtMath>
#include <QDebug>

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);

    QObject::connect(ui->SetButton,SIGNAL(clicked()),this,SLOT(Setbutton_clicked()));
    QObject::connect(ui->Reset,SIGNAL(clicked()),this,SLOT(Reset_Camera_Data()));
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

void ConfigDialog::Read_camera_data()//初始化相机参数显示
{
    string camera_data_dir = hx_options::intance()->current_data_dir;
    int pos = camera_data_dir.find_last_of("/");
    camera_data_dir = camera_data_dir.substr(0,pos-4);
    camera_data_dir +="bin/CameraParameter.cam";
    float x1,y1,z1,a11,a12,a13,a21,a22,a23,a31,a32,a33;
    float x2,y2,z2,b11,b12,b13,b21,b22,b23,b31,b32,b33;
    float x3,y3,z3,c11,c12,c13,c21,c22,c23,c31,c32,c33;
    float x4,y4,z4,d11,d12,d13,d21,d22,d23,d31,d32,d33;
    float x5,y5,z5,e11,e12,e13,e21,e22,e23,e31,e32,e33;
    float x6,y6,z6,f11,f12,f13,f21,f22,f23,f31,f32,f33;
    std::fstream camera_data(camera_data_dir,ios::in);
    if(!camera_data.is_open())
        return;
    camera_data>>x1>>y1>>z1>>a11>>a12>>a13>>a21>>a22>>a23>>a31>>a32>>a33
               >>x2>>y2>>z2>>b11>>b12>>b13>>b21>>b22>>b23>>b31>>b32>>b33
               >>x3>>y3>>z3>>c11>>c12>>c13>>c21>>c22>>c23>>c31>>c32>>c33
               >>x4>>y4>>z4>>d11>>d12>>d13>>d21>>d22>>d23>>d31>>d32>>d33
               >>x5>>y5>>z5>>e11>>e12>>e13>>e21>>e22>>e23>>e31>>e32>>e33
               >>x6>>y6>>z6>>f11>>f12>>f13>>f21>>f22>>f23>>f31>>f32>>f33;
    //设置文本
    ui->X1->setText(QString("%1").arg(QString::number(x1,'f',4)));
    ui->Y1->setText(QString("%1").arg(QString::number(y1,'f',4)));
    ui->Z1->setText(QString("%1").arg(QString::number(z1,'f',4)));
    ui->A11->setText(QString("%1").arg(QString::number(a11,'f',4)));
    ui->A12->setText(QString("%1").arg(QString::number(a12,'f',4)));
    ui->A13->setText(QString("%1").arg(QString::number(a13,'f',4)));
    ui->A21->setText(QString("%1").arg(QString::number(a21,'f',4)));
    ui->A22->setText(QString("%1").arg(QString::number(a22,'f',4)));
    ui->A23->setText(QString("%1").arg(QString::number(a23,'f',4)));
    ui->A31->setText(QString("%1").arg(QString::number(a31,'f',4)));
    ui->A32->setText(QString("%1").arg(QString::number(a32,'f',4)));
    ui->A33->setText(QString("%1").arg(QString::number(a33,'f',4)));//
    ui->X2->setText(QString("%1").arg(QString::number(x2,'f',4)));
    ui->Y2->setText(QString("%1").arg(QString::number(y2,'f',4)));
    ui->Z2->setText(QString("%1").arg(QString::number(z2,'f',4)));
    ui->B11->setText(QString("%1").arg(QString::number(b11,'f',4)));
    ui->B12->setText(QString("%1").arg(QString::number(b12,'f',4)));
    ui->B13->setText(QString("%1").arg(QString::number(b13,'f',4)));
    ui->B21->setText(QString("%1").arg(QString::number(b21,'f',4)));
    ui->B22->setText(QString("%1").arg(QString::number(b22,'f',4)));
    ui->B23->setText(QString("%1").arg(QString::number(b23,'f',4)));
    ui->B31->setText(QString("%1").arg(QString::number(b31,'f',4)));
    ui->B32->setText(QString("%1").arg(QString::number(b32,'f',4)));
    ui->B33->setText(QString("%1").arg(QString::number(b33,'f',4)));//
    ui->X3->setText(QString("%1").arg(QString::number(x3,'f',4)));
    ui->Y3->setText(QString("%1").arg(QString::number(y3,'f',4)));
    ui->Z3->setText(QString("%1").arg(QString::number(z3,'f',4)));
    ui->C11->setText(QString("%1").arg(QString::number(c11,'f',4)));
    ui->C12->setText(QString("%1").arg(QString::number(c12,'f',4)));
    ui->C13->setText(QString("%1").arg(QString::number(c13,'f',4)));
    ui->C21->setText(QString("%1").arg(QString::number(c21,'f',4)));
    ui->C22->setText(QString("%1").arg(QString::number(c22,'f',4)));
    ui->C23->setText(QString("%1").arg(QString::number(c23,'f',4)));
    ui->C31->setText(QString("%1").arg(QString::number(c31,'f',4)));
    ui->C32->setText(QString("%1").arg(QString::number(c32,'f',4)));
    ui->C33->setText(QString("%1").arg(QString::number(c33,'f',4)));//
    ui->X4->setText(QString("%1").arg(QString::number(x4,'f',4)));
    ui->Y4->setText(QString("%1").arg(QString::number(y4,'f',4)));
    ui->Z4->setText(QString("%1").arg(QString::number(z4,'f',4)));
    ui->D11->setText(QString("%1").arg(QString::number(d11,'f',4)));
    ui->D12->setText(QString("%1").arg(QString::number(d12,'f',4)));
    ui->D13->setText(QString("%1").arg(QString::number(d13,'f',4)));
    ui->D21->setText(QString("%1").arg(QString::number(d21,'f',4)));
    ui->D22->setText(QString("%1").arg(QString::number(d22,'f',4)));
    ui->D23->setText(QString("%1").arg(QString::number(d23,'f',4)));
    ui->D31->setText(QString("%1").arg(QString::number(d31,'f',4)));
    ui->D32->setText(QString("%1").arg(QString::number(d32,'f',4)));
    ui->D33->setText(QString("%1").arg(QString::number(d33,'f',4)));//
    ui->X5->setText(QString("%1").arg(QString::number(x5,'f',4)));
    ui->Y5->setText(QString("%1").arg(QString::number(y5,'f',4)));
    ui->Z5->setText(QString("%1").arg(QString::number(z5,'f',4)));
    ui->E11->setText(QString("%1").arg(QString::number(e11,'f',4)));
    ui->E12->setText(QString("%1").arg(QString::number(e12,'f',4)));
    ui->E13->setText(QString("%1").arg(QString::number(e13,'f',4)));
    ui->E21->setText(QString("%1").arg(QString::number(e21,'f',4)));
    ui->E22->setText(QString("%1").arg(QString::number(e22,'f',4)));
    ui->E23->setText(QString("%1").arg(QString::number(e23,'f',4)));
    ui->E31->setText(QString("%1").arg(QString::number(e31,'f',4)));
    ui->E32->setText(QString("%1").arg(QString::number(e32,'f',4)));
    ui->E33->setText(QString("%1").arg(QString::number(e33,'f',4)));//
    ui->X6->setText(QString("%1").arg(QString::number(x6,'f',4)));
    ui->Y6->setText(QString("%1").arg(QString::number(y6,'f',4)));
    ui->Z6->setText(QString("%1").arg(QString::number(z6,'f',4)));
    ui->F11->setText(QString("%1").arg(QString::number(f11,'f',4)));
    ui->F12->setText(QString("%1").arg(QString::number(f12,'f',4)));
    ui->F13->setText(QString("%1").arg(QString::number(f13,'f',4)));
    ui->F21->setText(QString("%1").arg(QString::number(f21,'f',4)));
    ui->F22->setText(QString("%1").arg(QString::number(f22,'f',4)));
    ui->F23->setText(QString("%1").arg(QString::number(f23,'f',4)));
    ui->F31->setText(QString("%1").arg(QString::number(f31,'f',4)));
    ui->F32->setText(QString("%1").arg(QString::number(f32,'f',4)));
    ui->F33->setText(QString("%1").arg(QString::number(f33,'f',4)));//

    camera_data.close();
}

void ConfigDialog::Setbutton_clicked()
{
    string x1 = ui->X1->text().toStdString();
    string y1 = ui->Y1->text().toStdString();
    string z1 = ui->Z1->text().toStdString();

    string a11 = ui->A11->text().toStdString();
    string a12 = ui->A12->text().toStdString();
    string a13 = ui->A13->text().toStdString();
    string a21 = ui->A21->text().toStdString();
    string a22 = ui->A22->text().toStdString();
    string a23 = ui->A23->text().toStdString();
    string a31 = ui->A31->text().toStdString();
    string a32 = ui->A32->text().toStdString();
    string a33 = ui->A33->text().toStdString();//左导航相机

    string x2 = ui->X2->text().toStdString();
    string y2 = ui->Y2->text().toStdString();
    string z2 = ui->Z2->text().toStdString();

    string b11 = ui->B11->text().toStdString();
    string b12 = ui->B12->text().toStdString();
    string b13 = ui->B13->text().toStdString();
    string b21 = ui->B21->text().toStdString();
    string b22 = ui->B22->text().toStdString();
    string b23 = ui->B23->text().toStdString();
    string b31 = ui->B31->text().toStdString();
    string b32 = ui->B32->text().toStdString();
    string b33 = ui->B33->text().toStdString();//右导航相机

    string x3 = ui->X3->text().toStdString();
    string y3 = ui->Y3->text().toStdString();
    string z3 = ui->Z3->text().toStdString();

    string c11 = ui->C11->text().toStdString();
    string c12 = ui->C12->text().toStdString();
    string c13 = ui->C13->text().toStdString();
    string c21 = ui->C21->text().toStdString();
    string c22 = ui->C22->text().toStdString();
    string c23 = ui->C23->text().toStdString();
    string c31 = ui->C31->text().toStdString();
    string c32 = ui->C32->text().toStdString();
    string c33 = ui->C33->text().toStdString();//左前避障相机

    string x4 = ui->X4->text().toStdString();
    string y4 = ui->Y4->text().toStdString();
    string z4 = ui->Z4->text().toStdString();

    string d11 = ui->D11->text().toStdString();
    string d12 = ui->D12->text().toStdString();
    string d13 = ui->D13->text().toStdString();
    string d21 = ui->D21->text().toStdString();
    string d22 = ui->D22->text().toStdString();
    string d23 = ui->D23->text().toStdString();
    string d31 = ui->D31->text().toStdString();
    string d32 = ui->D32->text().toStdString();
    string d33 = ui->D33->text().toStdString();//右前避障相机

    string x5 = ui->X5->text().toStdString();
    string y5 = ui->Y5->text().toStdString();
    string z5 = ui->Z5->text().toStdString();

    string e11 = ui->E11->text().toStdString();
    string e12 = ui->E12->text().toStdString();
    string e13 = ui->E13->text().toStdString();
    string e21 = ui->E21->text().toStdString();
    string e22 = ui->E22->text().toStdString();
    string e23 = ui->E23->text().toStdString();
    string e31 = ui->E31->text().toStdString();
    string e32 = ui->E32->text().toStdString();
    string e33 = ui->E33->text().toStdString();//左后避障相机

    string x6 = ui->X6->text().toStdString();
    string y6 = ui->Y6->text().toStdString();
    string z6 = ui->Z6->text().toStdString();

    string f11 = ui->F11->text().toStdString();
    string f12 = ui->F12->text().toStdString();
    string f13 = ui->F13->text().toStdString();
    string f21 = ui->F21->text().toStdString();
    string f22 = ui->F22->text().toStdString();
    string f23 = ui->F23->text().toStdString();
    string f31 = ui->F31->text().toStdString();
    string f32 = ui->F32->text().toStdString();
    string f33 = ui->F33->text().toStdString();//右后避障相机
    string camera_data_dir = hx_options::intance()->current_data_dir;
    int pos = camera_data_dir.find_last_of("/");
    camera_data_dir = camera_data_dir.substr(0,pos-4);
    camera_data_dir +="bin/CameraParameter.cam";
//    qDebug("%s",camera_data_dir.c_str());
    std::fstream data(camera_data_dir,ios::out);
    data<<x1<<"   "<<y1<<"   "<<z1<<endl;
    data<<a11<<"   "<<a12<<"   "<<a13<<endl;
    data<<a21<<"   "<<a22<<"   "<<a23<<endl;
    data<<a31<<"   "<<a32<<"   "<<a33<<endl<<endl;//
    data<<x2<<"   "<<y2<<"   "<<z2<<endl;
    data<<b11<<"   "<<b12<<"   "<<b13<<endl;
    data<<b21<<"   "<<b22<<"   "<<b23<<endl;
    data<<b31<<"   "<<b32<<"   "<<b33<<endl<<endl;//
    data<<x3<<"   "<<y3<<"   "<<z3<<endl;
    data<<c11<<"   "<<c12<<"   "<<c13<<endl;
    data<<c21<<"   "<<c22<<"   "<<c23<<endl;
    data<<c31<<"   "<<c32<<"   "<<c33<<endl<<endl;//
    data<<x4<<"   "<<y4<<"   "<<z4<<endl;
    data<<d11<<"   "<<d12<<"   "<<d13<<endl;
    data<<d21<<"   "<<d22<<"   "<<d23<<endl;
    data<<d31<<"   "<<d32<<"   "<<d33<<endl<<endl;//
    data<<x5<<"   "<<y5<<"   "<<z5<<endl;
    data<<e11<<"   "<<e12<<"   "<<e13<<endl;
    data<<e21<<"   "<<e22<<"   "<<e23<<endl;
    data<<e31<<"   "<<e32<<"   "<<e33<<endl<<endl;//
    data<<x6<<"   "<<y6<<"   "<<z6<<endl;
    data<<f11<<"   "<<f12<<"   "<<f13<<endl;
    data<<f21<<"   "<<f22<<"   "<<f23<<endl;
    data<<f31<<"   "<<f32<<"   "<<f33<<endl;//
    data.close();
    Read_camera_data();
    hx_che::intance()->loadCameraConfig();
}

void ConfigDialog::Reset_Camera_Data()
{
    string camera_data_dir = hx_options::intance()->current_data_dir;
    int pos = camera_data_dir.find_last_of("/");
    camera_data_dir = camera_data_dir.substr(0,pos-4);
    string camera_default_data_dir = camera_data_dir + "bin/CameraParameter_Default.txt";
    camera_data_dir +="bin/CameraParameter.cam";
    std::fstream data(camera_data_dir,ios::out);
    std::fstream data2(camera_default_data_dir,ios::in);
    char copydata;
    while(data2.get(copydata))
    {
    data << copydata;
    }
    data.close();
    data2.close();
    Read_camera_data();
    hx_che::intance()->loadCameraConfig();

}
