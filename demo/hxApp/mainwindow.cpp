#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMdiSubWindow>
#include <QFileDialog>
#include <hx/hx_core.h>
#include <hx/hx_terrain.h>
#include <hx/hx_che.h>
#include <hx/hx_stone.h>
#include <QMessageBox>
#include <QString>
#include <QDebug>
#include <QKeyEvent>
#include <hx/hx_options.h>
#include "network/server.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ////@全局初始化SSIM引擎
    if(!SSimInitCore())
        exit(0);


    ui->setupUi(this);
    QObject::connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(slot_open()));

    //QObject::connect(m_3DWidget, SIGNAL(on_mouse_event(QEvent*)), this, SLOT(slot_screen(QEvent*)));
    QObject::connect(ui->actionScreenShot, SIGNAL(triggered()), this, SLOT(slot_screen()));
    QObject::connect(ui->Button_Set_3,SIGNAL(clicked()),this,SLOT(slot_set()));
    QObject::connect(ui->actionCameraSet,SIGNAL(triggered()),this,SLOT(ConfigDialog_show()));

    //ui->mdiArea->geometry();

//    sw->resize(600,400);
//    sw->show();
    //sw->showMaximized();

    //sw->resize(600,400);

//    addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea,ui->dockWidgetSim);
//    addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea,ui->viewDockWidget);
//    addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea,ui->dockWidgetLight);

    tabifyDockWidget(ui->dockWidgetSim,ui->dockWidgetLight);
    tabifyDockWidget(ui->dockWidgetLight,ui->viewDockWidget);
    tabifyDockWidget(ui->viewDockWidget,ui->imageSimulationDockWidget);
    //ui->comboBox->setCurrentIndex(1);
    setMouseTracking(true);

    //text
    this->initText();

    this->showMaximized();
    m_3DWidget=NULL;
    m_server=new server();
}

void MainWindow::slot_open()
{

    QString file = QFileDialog::getOpenFileName(this,QStringLiteral("打开模型文件"),"../data");
    if (file.size() <= 0)
    {
        return;
    }

    m_3DWidget = new ViewerWidget2;
    //m_inspaceWidget=new IMainWidget(0);
    QObject::connect(m_3DWidget, SIGNAL(on_mouse_event(QEvent*)), this, SLOT(slot_mouse(QEvent*)));
    connect(m_3DWidget,SIGNAL(on_key_event(QEvent*)),this,SLOT(slot_KeyEvent(QEvent*)));


    QMdiSubWindow *sw = new QMdiSubWindow(this);
    QMdiSubWindow *swI = new QMdiSubWindow(this);

    sw->setWidget(m_3DWidget);
    sw->setWindowTitle(QString::fromLocal8Bit("火星三维仿真"));
    sw->setAttribute(Qt::WA_DeleteOnClose);
    ui->mdiArea->addSubWindow(sw);

//    swI->setWidget(m_inspaceWidget->m_3DWidget);
//    swI->setWindowTitle(QString::fromLocal8Bit("三维视图"));
//    swI->setAttribute(Qt::WA_DeleteOnClose);
//    ui->mdiArea->addSubWindow(swI);

    ui->mdiArea->showMaximized();

    const QRect &domain = ui->mdiArea->rect() ;
    int w = domain.width();
    int h = domain.height();
    qDebug() << w;
    qDebug() << h;
    sw->showMaximized();
    //sw->setGeometry(0,0,w/2,h);

    //sw->setGeometry(QStyle::visualRect(Qt::LeftToRight, domain, QRect(0,0,1,1)));
    //sw->show();

//    swI->setGeometry(w/2,0,w/2,h);
//    //swI->setGeometry(QStyle::visualRect(Qt::LeftToRight, domain, QRect(1,0,2,1)));
//    swI->show();


    hx_core::intance()->open_scene(file.toStdString());

    m_3DWidget->setScene(hx_core::intance()->get_scene_data());
    initCameraContralWin();
    initStoneWin();

    PickHandler* ph = m_3DWidget->getStonePickHandle();
    if(ph){
        StonePickCallback* spc = new StonePickCallback();
        spc->mw = this;
        ph->setPickCallback(spc);
    }
}

void MainWindow::slot_screen()
{
    QString file = QFileDialog::getSaveFileName(this,"文件","","IMAGE (*.bmp)");
    if (file.size() <= 0)
    {
        return;
    }
    bool flag =  this->m_3DWidget->saveScreenShot(file.toStdString());
    if(flag){
        QMessageBox::information(this,QString::fromLocal8Bit("提示"),file + QString::fromLocal8Bit("\n保存成功"));
    }else{
        QMessageBox::information(this,QString::fromLocal8Bit("提示"),file + QString::fromLocal8Bit("\n错误"));
    }
}

void MainWindow::slot_mouse(QEvent *ev)
{
    osg::Camera* ca = m_3DWidget->getCamera();
    if(!ca)return;

    osg::Vec3 e,c,u;
    ca->getViewMatrixAsLookAt(e,c,u);
    double h = hx_terrain::intance()->get_height( e.x() , e.y());
    //qDebug() << h - e.z() << "\n";

    QString qs;

    {
        qs=QString::fromLocal8Bit("视点位置: x: %1 y: %2  高度: %3 m").arg(e.x()).arg(e.y()).arg(e.z() - h,0,'g',3);
    }
    ui->statusBar->showMessage(qs);
//    QMouseEvent *mouseEvent=static_cast<QMouseEvent*>(ev);
//    if(mouseEvent->button()==Qt::LeftButton)
//    {
//        float s_posx=0.0,s_posy=0.0,s_posz=0.0;
//        bool is_choose=false;
//        m_3DWidget->chooseModelGetPos(s_posx,s_posy,s_posz,is_choose);
//        if(!is_choose)
//        {
//            s_posx=0.0;
//            s_posy=0.0;
//            s_posz=0.0;
//        }
//        ui->xlineEdit->setText(QString("%1").arg(s_posx));
//        ui->ylineEdit->setText(QString("%1").arg(s_posy));
//        ui->zlineEdit->setText(QString("%1").arg(s_posz));
//    }
}

void MainWindow::slot_KeyEvent(QEvent *e)
{
    QKeyEvent *keyEvent=static_cast<QKeyEvent*>(e);
    if(keyEvent->key()==Qt::Key_W)
    {
//        m_3DWidget->setVehicleAndCamera(0.0,0.2,ui->Value_A_3->text().toDouble(),ui->comboBox->currentIndex());
        initCameraContralWin();

    }
    else if(keyEvent->key()==Qt::Key_A)
    {
//        m_3DWidget->setVehicleAndCamera(-0.2,0.0,ui->Value_A_3->text().toDouble(),ui->comboBox->currentIndex());
        initCameraContralWin();
    }
    else if (keyEvent->key()==Qt::Key_S) {
//        m_3DWidget->setVehicleAndCamera(0.0,-0.2,ui->Value_A_3->text().toDouble(),ui->comboBox->currentIndex());
        initCameraContralWin();
    }
    else if (keyEvent->key()==Qt::Key_D) {
//        m_3DWidget->setVehicleAndCamera(0.2,0.0,ui->Value_A_3->text().toDouble(),ui->comboBox->currentIndex());
        initCameraContralWin();
    }
    else if(keyEvent->key()==Qt::Key_Up)
        initCameraContralWin();
    else if (keyEvent->key()==Qt::Key_Down) {
        initCameraContralWin();
    }
    else if (keyEvent->key()==Qt::Key_Left || keyEvent->key()==Qt::Key_Right) {
        initCameraContralWin();
    }
}

void MainWindow::slot_set()
{

    double x = ui->Value_X_3->text().toDouble();
    double y = ui->Value_Y_3->text().toDouble();
    double a = ui->Value_A_3->text().toDouble();

    if(x<1||x>999)
    {
        QMessageBox::warning(NULL,"提示","输入的参数超出范围，请检查并重新输入",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    }
    else
    {
        if(y<1||y>999)
        {
           QMessageBox::warning(NULL,"提示","输入的参数超出范围，请检查并重新输入",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        }
        else
        {
        if(a<0||a>360)
        {
           QMessageBox::warning(NULL,"提示","输入的参数超出范围，请检查并重新输入",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        }

        else
        {
//            m_3DWidget->setVehicleAndCamera(x-m_3DWidget->getVehicleX(),y-m_3DWidget->getVehicleY(),a,ui->comboBox->currentIndex());
            hx_che::intance()->move(x,y,a);
            initCameraContralWin();
        }
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
//初始化石头编辑界面
void MainWindow::initStoneWin()
{
    ui->nbigEdt->setText(QString("%1").arg(hx_stone::intance()->getBigCount()));
    ui->nmidEdt->setText(QString("%1").arg(hx_stone::intance()->getMiddleCount()));
    ui->nsmaEdt->setText(QString("%1").arg(hx_stone::intance()->getSmallCount()));
    ui->zoomEdit->setText(QString("%1").arg(1));
    ui->rotateEdit->setText(QString("%1").arg(10));
    ui->changelineEdit->setText(QString("%1").arg(2));
    ui->lengthEdit->setText(QString("%1").arg(1));
    ui->widthEdit->setText(QString("%1").arg(1));
    ui->heightEdit->setText(QString("%1").arg(1));
    ui->rollEdit->setText(QString("%1").arg(10));
    ui->pitchEdit->setText(QString("%1").arg(10));
    ui->yawEdit->setText(QString("%1").arg(10));
}

void MainWindow::on_northBtn_clicked()
{
    if(ui->changelineEdit->text().isEmpty())
        return;
    float value_change=ui->changelineEdit->text().toFloat();
    m_3DWidget->chooseModelTranslate(0.0,value_change,0.0);
}

void MainWindow::on_westBtn_clicked()
{
    if(ui->changelineEdit->text().isEmpty())
        return;
    float value_change=ui->changelineEdit->text().toFloat();
    m_3DWidget->chooseModelTranslate(-value_change,0.0,0.0);
}

void MainWindow::on_eastBtn_clicked()
{
    if(ui->changelineEdit->text().isEmpty())
        return;
    float value_change=ui->changelineEdit->text().toFloat();
    m_3DWidget->chooseModelTranslate(value_change,0.0,0.0);
}

void MainWindow::on_highBtn_clicked()
{
    if(ui->changelineEdit->text().isEmpty())
        return;
    float value_change=ui->changelineEdit->text().toFloat();
    m_3DWidget->chooseModelTranslate(0.0,0.0,value_change);
}

void MainWindow::on_sourthBtn_clicked()
{
    if(ui->changelineEdit->text().isEmpty())
        return;
    float value_change=ui->changelineEdit->text().toFloat();
    m_3DWidget->chooseModelTranslate(0.0,-value_change,0.0);
}

void MainWindow::on_lowBtn_clicked()
{
    if(ui->changelineEdit->text().isEmpty())
        return;
    float value_change=ui->changelineEdit->text().toFloat();
    m_3DWidget->chooseModelTranslate(0.0,0.0,-value_change);
}

void MainWindow::on_zoomOutBtn_clicked()
{
    float leng=0.0;
    float widt=0.0;
    float heig=0.0;
    if(ui->lengthEdit->text().isEmpty() && ui->widthEdit->text().isEmpty() && ui->heightEdit->text().isEmpty())
    {
        if(!ui->zoomEdit->text().isEmpty())
        {
            leng=ui->zoomEdit->text().toFloat();
            widt=leng;
            heig=leng;
        }else {
            return;
        }
    }else {
        leng=ui->lengthEdit->text().toFloat();
        widt=ui->widthEdit->text().toFloat();
        heig=ui->heightEdit->text().toFloat();
    }
    m_3DWidget->chooseModelScale(leng,widt,heig);
}


void MainWindow::on_clockwiseBtn_clicked()
{
    float degree_x=0.0;
    float degree_y=0.0;
    float degree_z=0.0;
    if(ui->rollEdit->text().isEmpty() && ui->pitchEdit->text().isEmpty() && ui->yawEdit->text().isEmpty())
    {
        if(!ui->rotateEdit->text().isEmpty())
        {
            degree_x=ui->rotateEdit->text().toFloat();
            degree_y=degree_x;
            degree_z=degree_x;
        }
    }else {
        degree_x=ui->rollEdit->text().toFloat();
        degree_y=ui->pitchEdit->text().toFloat();
        degree_z=ui->yawEdit->text().toFloat();
    }
    m_3DWidget->chooseModelRotate(degree_x,1,0,0);
    m_3DWidget->chooseModelRotate(degree_y,0,1,0);
    m_3DWidget->chooseModelRotate(degree_z,0,0,1);
}

void MainWindow::on_deleteBtn_clicked()
{
    m_3DWidget->deleteStone();
}

void MainWindow::on_saveStoneBtn_clicked()
{
    if(hx_stone::intance()->saveAllStoneState())
        qDebug()<<"Success";
    else
        qDebug()<<"fail";
}

void MainWindow::on_positionBtn_clicked()
{
    float pos_x=0.0;
    float pos_y=0.0;
    float pos_z=0.0;
    pos_x=ui->xlineEdit->text().toFloat();
    pos_y=ui->ylineEdit->text().toFloat();
    pos_z=ui->zlineEdit->text().toFloat();
    if(pos_x==1000.0 && pos_y==1000.0)
        pos_y=999.0;
    m_3DWidget->chooseModelSetPos(pos_x,pos_y,pos_z);
}


void MainWindow::on_produceBtn_clicked()
{
    if(ui->nbigEdt->text().isEmpty() && ui->nmidEdt->text().isEmpty() && ui->nsmaEdt->text().isEmpty())
        return;
    unsigned int big_num=ui->nbigEdt->text().toUInt();
    unsigned int mid_num=ui->nmidEdt->text().toUInt();
    unsigned int sma_num=ui->nsmaEdt->text().toUInt();
    if(big_num==0&&mid_num==0&&sma_num==0)
        return;
    else
        m_3DWidget->produceStoneFile(big_num,mid_num,sma_num);

    hx_stone::intance()->produceStone_osgb_file();
//    hx_stone::intance()->init();
}

#include <hx/hx_handle.h>
#include <hx/lvds/handles.h>
#include <osg/Matrix>

#include <hx/socket/handle_dem.h>

void MainWindow::on_text_takepic_navi()
{
//    handle_takepic handle;
//    handle.setMode(1);
//    handle.handle(iodata());
//    QMessageBox::information(this,"INFO","拍照完成");

    handle_dem handler;
    handle_dem::SocketDemReqCommand drs;
    drs.header.commandword = handle_dem::DemReqCommand;
    drs.header.index = 1;
    drs.xmin = 400;
    drs.xmax = 600;
    drs.ymax = 600;
    drs.ymin = 400;
    drs.step = 1;
    iodata req = drs.getData();
    iodata res = handler.handle(req);

    if(!res.data)
    {
        //QString err = QString::fromStdString(res.getErrorString());
        res.release();
        QMessageBox::information(this,"INFO","err");
        return;
    }

    handle_dem::SocketDemResponseCommand rescmd;
    if(!rescmd.read(res))
    {
        res.release();
        QMessageBox::information(this,"INFO","err");
        return;
    }


    QFile data(std::string("D:/dem.dat").c_str());
    if (data.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&data);
        //out << count << "\n";
        out << rescmd.x << "\n";
        out << rescmd.y << "\n";
        int size = rescmd.x * rescmd.y;

        for(int i = 0; i < size; i++){
            out
#ifdef HANDLE_DEM_OUTXY
             << QString("%1").arg(rescmd.points[i].x,0,'g',12) << "\t"
             << QString("%1").arg(rescmd.points[i].y,0,'g',12) << "\t"
#endif
             << QString("%1").arg(rescmd.points[i].z,0,'g',12) << "\t"
             <<  QString("%1").arg((int)rescmd.marks[i]) << "\n";
        }
        out.flush();
    }
    req.release();
    res.release();
    data.close();

//    osg::Matrix mv;
//    mv.makeLookAt(osg::Vec3(501,512,0),osg::Vec3(501,512,-1000),osg::Vec3(1,0,0));

//    osg::Matrix prj;
//    prj.makeOrtho(-10,10,-10,10,1,10000);
//    //prj.makePerspective(60,1.0,1,10000);
//    sim_core* sm = hx_core::intance()->get_simcore();
//    sm->option().mv_matrix = mv;
//    sm->option().proj_matrix = prj;
//    sm->option().image_width = 512;
//    sm->option().simtype = sim_option::SIM_DepthNoChe;
//    osg::Image* img = sm->getImage();
//    bool res = false;
//    if(img){
//        res = osgDB::writeImageFile(*img,std::string("D:/dem.tiff"));
//    }

//    sm->option().simtype = sim_option::SIM_DepthStone;
//    img = sm->getImage();
//    if(img){
//        res = osgDB::writeImageFile(*img,std::string("D:/dem2.tiff"));
//    }

}

void MainWindow::on_text_outpic_navi()
{
    handle_outimg_navi outimg;
    iodata res = outimg.handle(iodata());
    if(res.size <= 0 || !res.data){
        QMessageBox::information(this,"INFO","输出影像失败");
    }
    QString file = QFileDialog::getSaveFileName(this,"文件","","影像流(*.dat)");
    if (file.size() <= 0)
    {
        return;
    }

    QFile f(file);
    if(f.open(QIODevice::WriteOnly))
    {
        f.write(res.data,res.size);
        f.close();
        QMessageBox::information(this,"INFO","输出影像成功");
    }
    else
    {
        QMessageBox::information(this,"INFO","输出影像失败");
    }

}
#include <hx/camerastatus.h>

void MainWindow::on_text_savepic_navi()
{

    QString file = QFileDialog::getSaveFileName(this,"文件","","IMAGE (*.bmp)");
    if (file.size() <= 0)
    {
        return;
    }
    osg::Image* img = CameraStatus::navi_camera()->image;
    bool flag = false;
    if(img){
        flag = osgDB::writeImageFile(*img,file.toStdString());
    }
    if(flag){
        QMessageBox::information(this,"提示",file + "\n保存成功");
    }else{
        QMessageBox::information(this,"提示",file + "\n错误");
    }
}

void MainWindow::on_text_takepic_avoid()
{
    handle_takepic handle;
    handle.setMode(3);
    handle.handle(iodata());
    QMessageBox::information(this,"INFO","拍照完成");
}

void MainWindow::on_text_savepic_avoid()
{
    QString file = QFileDialog::getSaveFileName(this,"文件","","IMAGE (*.bmp)");
    if (file.size() <= 0)
    {
        return;
    }
    osg::Image* img = CameraStatus::avoid_camera()->image;
    bool flag = false;
    if(img){
        flag = osgDB::writeImageFile(*img,file.toStdString());
    }
    if(flag){
        QMessageBox::information(this,"提示",file + "\n保存成功");
    }else{
        QMessageBox::information(this,"提示",file + "\n错误");
    }
}
void MainWindow::initText()
{
    connect(ui->action_takepic_navi,SIGNAL(triggered()),this,SLOT(on_text_takepic_navi()));
    connect(ui->action_outpic_navi,SIGNAL(triggered()),this,SLOT(on_text_outpic_navi()));
    connect(ui->action_save_img_navi,SIGNAL(triggered()),this,SLOT(on_text_savepic_navi()));

    connect(ui->action_takepic_avoid,SIGNAL(triggered()),this,SLOT(on_text_takepic_avoid()));
    //connect(ui->action_outpic_navi,SIGNAL(triggered()),this,SLOT(on_text_outpic_navi()));
    connect(ui->action_save_img_avoid,SIGNAL(triggered()),this,SLOT(on_text_savepic_avoid()));

}


void MainWindow::initCameraContralWin()
{
    MyPosRotate vehiclePosRot=m_3DWidget->getVehiclePosRot();
    ui->Value_X_3->setText(QString("%1").arg(QString::number(vehiclePosRot.position[0],'f',2)));
    ui->Value_Y_3->setText(QString("%1").arg(QString::number(vehiclePosRot.position[1],'f',2)));
    ui->Value_A_3->setText(QString("%1").arg(QString::number(vehiclePosRot.rotate[2],'f',0)));
    MyPosRotate navigation1=m_3DWidget->getNavigation1();
    ui->navigation1X->setText(QString("%1").arg(QString::number(navigation1.position[0],'f',2)));
    ui->navigation1Y->setText(QString("%1").arg(QString::number(navigation1.position[1],'f',2)));
    ui->navigation1Z->setText(QString("%1").arg(QString::number(navigation1.position[2],'f',1)));
    ui->navigation1roll->setText(QString("%1").arg(QString::number(navigation1.rotate[1],'f',2)));
    ui->navigation1pitch->setText(QString("%1").arg(QString::number(navigation1.rotate[0],'f',2)));
    ui->navigation1yaw->setText(QString("%1").arg(QString::number(navigation1.rotate[2],'f',2)));
    MyPosRotate navigation2=m_3DWidget->getNavigation2();
    ui->navigation2X->setText(QString("%1").arg(QString::number(navigation2.position[0],'f',2)));
    ui->navigation2Y->setText(QString("%1").arg(QString::number(navigation2.position[1],'f',2)));
    ui->navigation2Z->setText(QString("%1").arg(QString::number(navigation2.position[2],'f',1)));
    ui->navigation2roll->setText(QString("%1").arg(QString::number(navigation2.rotate[1],'f',2)));
    ui->navigation2pitch->setText(QString("%1").arg(QString::number(navigation2.rotate[0],'f',2)));
    ui->navigation2yaw->setText(QString("%1").arg(QString::number(navigation2.rotate[2],'f',2)));
    MyPosRotate avoid1=m_3DWidget->getAvoid1();
    ui->forwardAvoid1X->setText(QString("%1").arg(QString::number(avoid1.position[0],'f',2)));
    ui->forwardAvoid1Y->setText(QString("%1").arg(QString::number(avoid1.position[1],'f',2)));
    ui->forwardAvoid1Z->setText(QString("%1").arg(QString::number(avoid1.position[2],'f',1)));
    ui->forwardAvoid1roll->setText(QString("%1").arg(QString::number(avoid1.rotate[1],'f',2)));
    ui->forwardAvoid1pitch->setText(QString("%1").arg(QString::number(avoid1.rotate[0],'f',2)));
    ui->forwardAvoid1raw->setText(QString("%1").arg(QString::number(avoid1.rotate[2],'f',2)));
    MyPosRotate avoid2=m_3DWidget->getAvoid2();
    ui->forwardAvoid2X->setText(QString("%1").arg(QString::number(avoid2.position[0],'f',2)));
    ui->forwardAvoid2Y->setText(QString("%1").arg(QString::number(avoid2.position[1],'f',2)));
    ui->forwardAvoid2Z->setText(QString("%1").arg(QString::number(avoid2.position[2],'f',1)));
    ui->forwardAvoid2roll->setText(QString("%1").arg(QString::number(avoid2.rotate[1],'f',2)));
    ui->forwardAvoid2pitch->setText(QString("%1").arg(QString::number(avoid2.rotate[0],'f',2)));
    ui->forwardAvoid2raw->setText(QString("%1").arg(QString::number(avoid2.rotate[2],'f',2)));
    MyPosRotate back_avoid1=m_3DWidget->getBackwardAvoid1();
    ui->backwardAvoid1X->setText(QString("%1").arg(QString::number(back_avoid1.position[0],'f',2)));
    ui->backwardAvoid1Y->setText(QString("%1").arg(QString::number(back_avoid1.position[1],'f',2)));
    ui->backwardAvoid1Z->setText(QString("%1").arg(QString::number(back_avoid1.position[2],'f',1)));
    ui->backwardAvoid1roll->setText(QString("%1").arg(QString::number(back_avoid1.rotate[1],'f',2)));
    ui->backwardAvoid1pitch->setText(QString("%1").arg(QString::number(back_avoid1.rotate[0],'f',2)));
    ui->backwardAvoid1raw->setText(QString("%1").arg(QString::number(back_avoid1.rotate[2],'f',2)));
    MyPosRotate back_avoid2=m_3DWidget->getBackwardAvoid2();
    ui->backwardAvoid2X->setText(QString("%1").arg(QString::number(back_avoid2.position[0],'f',2)));
    ui->backwardAvoid2Y->setText(QString("%1").arg(QString::number(back_avoid2.position[1],'f',2)));
    ui->backwardAvoid2Z->setText(QString("%1").arg(QString::number(back_avoid2.position[2],'f',1)));
    ui->backwardAvoid2roll->setText(QString("%1").arg(QString::number(back_avoid2.rotate[1],'f',2)));
    ui->backwardAvoid2pitch->setText(QString("%1").arg(QString::number(back_avoid2.rotate[0],'f',2)));
    ui->backwardAvoid2raw->setText(QString("%1").arg(QString::number(back_avoid2.rotate[2],'f',2)));
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    if(!m_3DWidget)
        return;
    qDebug()<<index;
    m_3DWidget->setCameraMode(index);
    //m_3DWidget->setVehicleAndCamera(0.0,0.0,0.0,index);
}

#include <hx/env_builder.h>
void MainWindow::on_pushButton_light_clicked()
{
    double vx = ui->lineEdit_light_x->text().toDouble();
    double vy = ui->lineEdit_light_y->text().toDouble();
    double vz = ui->lineEdit_light_z->text().toDouble();
    env_builder::intance()->setLightDir(vx,vy,vz);

}

void MainWindow::ConfigDialog_show()
{
    dialog = new ConfigDialog(this);
//    dialog->setModal(true);
    dialog->show();
    dialog->Read_camera_data();

}


void MainWindow::updatePickStone(osg::Node *n)
{
    float pos_x=0.0,pos_y=0.0,pos_z=0.0;
    osg::ref_ptr<osg::MatrixTransform> mt=dynamic_cast<osg::MatrixTransform*>(n);
    osg::Matrix t_matrix=mt->getMatrix();
    osg::Vec3 pos_v=t_matrix.getTrans();
    pos_x=pos_v.x();
    pos_y=pos_v.y();
    pos_z=pos_v.z();
    ui->xlineEdit->setText(QString("%1").arg(pos_x));
    ui->ylineEdit->setText(QString("%1").arg(pos_y));
    ui->zlineEdit->setText(QString("%1").arg(pos_z));
}


StonePickCallback::~StonePickCallback()
{

}

void StonePickCallback::pick(osg::Node *node)
{
    if(this->mw)
    {
        mw->updatePickStone(node);
    }
}
//导航相机a拍照
void MainWindow::on_navi_a_clicked()
{
    handle_takepic handle;
    handle.setMode(1);
    handle.handle(iodata());

    QString file = QFileDialog::getSaveFileName(this,"文件","","IMAGE (*.bmp)");
    if (file.size() <= 0)
    {
        return;
    }
    osg::Image* img = CameraStatus::navi_camera()->image;
    bool flag = false;
    if(img){
        flag = osgDB::writeImageFile(*img,file.toStdString());
    }
    if(flag){
        QMessageBox::information(this,"提示",file + "\n保存成功");
    }else{
        QMessageBox::information(this,"提示",file + "\n错误");
    }
}
//导航相机b拍照
void MainWindow::on_navi_b_clicked()
{
    handle_takepic handle;
    handle.setMode(2);
    handle.handle(iodata());

    QString file = QFileDialog::getSaveFileName(this,"文件","","IMAGE (*.bmp)");
    if (file.size() <= 0)
    {
        return;
    }
    osg::Image* img = CameraStatus::navi_camera2()->image;
    bool flag = false;
    if(img){
        flag = osgDB::writeImageFile(*img,file.toStdString());
    }
    if(flag){
        QMessageBox::information(this,"提示",file + "\n保存成功");
    }else{
        QMessageBox::information(this,"提示",file + "\n错误");
    }
}
//前避障相机a拍照
void MainWindow::on_foravoid_a_clicked()
{
    handle_takepic handle;
    handle.setMode(3);
    handle.handle(iodata());

    QString file = QFileDialog::getSaveFileName(this,"文件","","IMAGE (*.bmp)");
    if (file.size() <= 0)
    {
        return;
    }
    osg::Image* img = CameraStatus::avoid_camera()->image;
    bool flag = false;
    if(img){
        flag = osgDB::writeImageFile(*img,file.toStdString());
    }
    if(flag){
        QMessageBox::information(this,"提示",file + "\n保存成功");
    }else{
        QMessageBox::information(this,"提示",file + "\n错误");
    }
}
//前避障相机b拍照
void MainWindow::on_foravoid_b_clicked()
{
    handle_takepic handle;
    handle.setMode(4);
    handle.handle(iodata());

    QString file = QFileDialog::getSaveFileName(this,"文件","","IMAGE (*.bmp)");
    if (file.size() <= 0)
    {
        return;
    }
    osg::Image* img = CameraStatus::avoid_camera2()->image;
    bool flag = false;
    if(img){
        flag = osgDB::writeImageFile(*img,file.toStdString());
    }
    if(flag){
        QMessageBox::information(this,"提示",file + "\n保存成功");
    }else{
        QMessageBox::information(this,"提示",file + "\n错误");
    }
}
//后避障相机a拍照
void MainWindow::on_backavoid_a_clicked()
{
    handle_takepic handle;
    handle.setMode(5);
    handle.handle(iodata());

    QString file = QFileDialog::getSaveFileName(this,"文件","","IMAGE (*.bmp)");
    if (file.size() <= 0)
    {
        return;
    }
    osg::Image* img = CameraStatus::back_avoid_camera1()->image;
    bool flag = false;
    if(img){
        flag = osgDB::writeImageFile(*img,file.toStdString());
    }
    if(flag){
        QMessageBox::information(this,"提示",file + "\n保存成功");
    }else{
        QMessageBox::information(this,"提示",file + "\n错误");
    }
}
//后避障相机b拍照
void MainWindow::on_backavoid_b_clicked()
{
    handle_takepic handle;
    handle.setMode(6);
    handle.handle(iodata());

    QString file = QFileDialog::getSaveFileName(this,"文件","","IMAGE (*.bmp)");
    if (file.size() <= 0)
    {
        return;
    }
    osg::Image* img = CameraStatus::back_avoid_camera2()->image;
    bool flag = false;
    if(img){
        flag = osgDB::writeImageFile(*img,file.toStdString());
    }
    if(flag){
        QMessageBox::information(this,"提示",file + "\n保存成功");
    }else{
        QMessageBox::information(this,"提示",file + "\n错误");
    }
}

void MainWindow::on_userOutImageBtn_clicked()
{
    MyPosRotate m_pr;
    if(ui->userCameraPosXEdt->text().isEmpty() ||
            ui->userCameraPosYEdt->text().isEmpty() ||
            ui->userCameraPosZEdt->text().isEmpty() ||
            ui->userCamerarollEdt->text().isEmpty() ||
            ui->userCamerapitchEdt->text().isEmpty() ||
            ui->userCamerayawEdt->text().isEmpty())
    {
        QMessageBox::information(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("参数不能为空，请检查参数！"));
        return;
    }

    float pos_x=ui->userCameraPosXEdt->text().toFloat();
    float pos_y=ui->userCameraPosYEdt->text().toFloat();
    float pos_z=ui->userCameraPosZEdt->text().toFloat();
    float rotate_roll=ui->userCamerarollEdt->text().toFloat();
    float rotate_pitch=ui->userCamerapitchEdt->text().toFloat();
    float rotate_yaw=ui->userCamerayawEdt->text().toFloat();
    if(pos_x>1000.0 || pos_x<0.0)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("相机位置X超过范围"));
        return;
    }
    if(pos_y>1000.0 || pos_y<0.0)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("相机位置Y超过范围"));
        return;
    }

    m_pr.position=osg::Vec3(pos_x,pos_y,pos_z);
    m_pr.rotate=osg::Vec3(rotate_roll,rotate_pitch,rotate_yaw);

    if(ui->userFovEdt->text().isEmpty())
    {
        QMessageBox::information(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("视场角不能为空"));
        return;
    }
    double fov=ui->userFovEdt->text().toDouble();
    if(fov<0.0 || fov >360.0)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("视场角超过范围"));
        return;
    }
    unsigned int navi_avoid=ui->userCameraCoBox->currentIndex();
    navi_avoid++;
    handle_takepic handle;
    handle.handle(iodata(),m_pr,navi_avoid,fov);

    osg::Image* img=CameraStatus::user_camera()->image;
    QString file = QFileDialog::getSaveFileName(this,QString::fromLocal8Bit("文件"),"","IMAGE (*.bmp)");
    if (file.size() <= 0)
    {
        return;
    }
    bool flag = false;

    //0:彩度图像
    //1:灰色图像
    if(ui->userImageCoBox->currentIndex()==1)
    {
        int width=img->s();
        int height=img->t();
//        int bytePerLine=(width*24+31)/8;
//        unsigned int allDataCount=bytePerLine*height;
        unsigned char *graydata=new unsigned char[width*height];

        if(img->getOrigin()==osg::Image::BOTTOM_LEFT)
        {
            for(int s=width-1;s>=0;--s)
            {
                for(int t=height-1;t>=0;--t)
                {
                    osg::Vec4 color=img->getColor(s,t);
                    float fgray = color.r() * 0.299 + color.g() * 0.587 + color.b() * 0.114;
//                    graydata[allDataCount-t*bytePerLine-(width-s)*3]=fgray*256.0f;
//                    graydata[allDataCount-t*bytePerLine-(width-s)*3-1]=fgray*256.0f;
//                    graydata[allDataCount-t*bytePerLine-(width-s)*3-2]=fgray*256.0f;
                    graydata[t*width+s]=fgray*256.0f;
                }
            }
        }
        else
        {
            for(int s=0;s<width;++s)
            {
                for(int t=0;t<height;++t)
                {
                    osg::Vec4 color=img->getColor(s,t);
                    float fgray = color.r() * 0.299 + color.g() * 0.587 + color.b() * 0.114;
//                    graydata[t*bytePerLine+s*3]=fgray*256.0f;
//                    graydata[t*bytePerLine+s*3+1]=fgray*256.0f;
//                    graydata[t*bytePerLine+s*3+2]=fgray*256.0f;
                    graydata[t*width+s]=fgray*256.0f;
                }
            }
        }
        osg::Image *grayImage=new osg::Image();
//        grayImage->allocateImage(width,height,img->r(),GL_RED,GL_UNSIGNED_BYTE);
//        grayImage->setData(graydata,osg::Image::NO_DELETE);
        grayImage->setImage(width,height,img->r(),img->getInternalTextureFormat(),
                            GL_RED,GL_UNSIGNED_BYTE,graydata,osg::Image::NO_DELETE);
//        QImage *grayImage=new QImage(graydata,width,height,bytePerLine,QImage::Format_RGB888);
//        flag=grayImage->save(file);
        flag = osgDB::writeImageFile(*grayImage,file.toStdString());
        delete[]graydata;
    }
    else if(ui->userImageCoBox->currentIndex()==0)
    {
        if(img){
            flag = osgDB::writeImageFile(*img,file.toStdString());
        }
    }


    if(flag){
        QMessageBox::information(this,QString::fromLocal8Bit("提示"),file + QString::fromLocal8Bit("\n保存成功"));
    }else{
        QMessageBox::information(this,QString::fromLocal8Bit("提示"),file + QString::fromLocal8Bit("\n错误"));
    }
}

//TODO:测试函数，用完删除
void MainWindow::on_testBtn_clicked()
{
    MarsVehicleParameter mvp;
    mvp.position=osg::Vec3(500,499,-2336.3);
    mvp.rotate=osg::Vec3(6.06,4.05,0.0);
    mvp.mast_pitch=0.0;
    mvp.mast_yaw=0.0;
    hx_che::intance()->setMarsVehicleMast(mvp);
}

void MainWindow::on_actionServer_triggered()
{
    m_server->setWindowTitle(QString::fromLocal8Bit("服务器设置"));
    m_server->show();
}
