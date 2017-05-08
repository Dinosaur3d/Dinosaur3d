#include "IMainWidget.h"
#include <SSimMainApp/SSimCore.h>
#include <SSimView/BodyObject.h>
#include <QSurfaceFormat>
#include <QGLFormat>
#include <QQmlContext>
#include <SSimView/SSim3DView.h>
#include <SSimView/SSim2DView.h>
#include <QDomElement>
#include <QDomText>
#include <QDebug>
#include <QCloseEvent>
#include <QSqlQuery>
#include <QTextStream>
#include <QFile>
#include <SSimDlgServices/terrainpatheditwidget.h>

//如果使用VS编译器，需要将代码强制转换为UTF-8,并将文件保存为UTF-8编码，才能正确显示中文
#if defined(_MSC_VER)&&(_MSC_VER>=1600)
#pragma execution_character_set("utf-8")
#endif

IMainWidget::IMainWidget(QWidget *parent)
    : QWidget(parent)
{
    ssimCore = new SSimCore;

    ////-----步骤2：初始化时间设置-----------------------------
    ////@（1）初始化时间，初始化定时器,默认设置当前系统时间为计算机时间
    ssimCore->initTime();

    ////@      设置当前系统时间，用户可以任意更改
    double now = ssim::JdToMjd(ssim::CalendarToJd(QDateTime::currentDateTime().toUTC()));
    ssimCore->setTime(now);
    connect(ssimCore, SIGNAL(timeChanged()), this, SLOT(timeChanged()));
    connect(ssimCore, SIGNAL(scenarioPropagated()), this, SLOT(scenarioPropagated()));

    SSim3DView* p3DView = ssimCore->create3DView();
    m_3DView = p3DView;

    m_3DWidget = QWidget::createWindowContainer(p3DView);
    m_azimuth = m_elevation = m_range = 0;


    connect(m_3DView, SIGNAL(initializeGLisDone()), this, SLOT(initializeGLisDone()));
}

IMainWidget::~IMainWidget()
{

}

void IMainWidget::timeChanged()
{
    //相机窗口视角切换
    if(m_3DView != NULL && ssimCore->getPropagatedScenario())
    {
//        QString fromName = m_pCameraParaDlg->m_observerComboBox;
//        QString toName = m_pCameraParaDlg->m_targetComboBox;
//        if(!fromName.isEmpty() && !toName.isEmpty())
//        {
//            //去除三维视图干扰信息
////             m_3DCameraView->setAllOrbits(false);
////             m_3DCameraView->setStars(false);
////             m_3DCameraView->setClouds(false);
////             m_3DCameraView->setAtmospheresVisible(false);
////             m_3DCameraView->setsysInfoVisibility(false);
////             m_3DCameraView->setSensorEnable(false);
////             m_3DCameraView->setPlanetLabels(false);

////             m_3DCameraView->setCamera(m_pCameraParaDlg->m_fov* ssim::DEG2RAD, fromName, toName, 0);// 吕亮改

//        }
    }


    ////@获取指定的地面目标
    GroundObject* gObj = NULL;

    double a = 0,e = 0,r = 0;

    //获得当前时刻，MJD格式
    double t = ssimCore->getCurrentMJD();

    m_targetName = "";
    //获得预报场景
    PropagatedScenario* scenario = ssimCore->getPropagatedScenario();
    if( scenario != NULL && m_3DView != NULL)
    {
        m_selectBodyName = m_3DView->getSelectedBody()->name();
        foreach (GroundObject* groundObj, scenario->groundObjects())
        {
            //进行名称判断
            if (groundObj->name() == m_selectBodyName)
            {
                gObj = groundObj;
                //判断该目标有没有Target类型传感器

                ScenarioGroundStation* groundStation = dynamic_cast<ScenarioGroundStation*>(groundObj->participant());
                if (groundStation)
                {
                    unsigned int sensorIndex = 0;
                    foreach (QSharedPointer<ScenarioAbstractPayloadType> payload, groundStation->PayloadSet()->AbstractPayload())
                    {
                        ScenarioTransmitterPayloadType* transmitter = dynamic_cast<ScenarioTransmitterPayloadType*>(payload.data());
                        if (transmitter && transmitter->Transmitter()->PointingDirection()->pointingType().toLower() == "followtarget")
                        {

                            m_targetName = transmitter->Transmitter()->PointingDirection()->target();
                        }
                    }
                }
            }
        }
        if(m_targetName != "")
        {
            ////@获取指定的空间目标
            SpaceObject* sObj = NULL;
            foreach (SpaceObject* spaceObj, scenario->spaceObjects())
            {
                if (spaceObj->name() == m_targetName)
                {
                    sObj = spaceObj;
                }
            }
            ////@获取给定时间段内的俯仰角,方位角,斜距
            m_elevation = 0;
            if (gObj&&sObj)
            {
                m_elevation = gObj->elevationAngle(sObj, t);////@俯仰角,DEG
                m_azimuth = gObj->azimuthAngle(sObj, t); ////@方位角,DEG
                m_range = gObj->getRange(sObj, t);     ////@斜距,KM
            }

            if(m_elevation>=5)
            {
                ////@ 获得3d窗口
                for(int i = 0;i<ssimCore->getViewList().size();i++)
                {

                   SSim3DView *child3D = dynamic_cast<SSim3DView *>(ssimCore->getViewList().at(i));
                   if (child3D!=NULL)
                   {

                       //水平角
                       //含中文的QString转换为char的方法
                       std::string name_std = gObj->name().toStdString();
                       const char *name = name_std.c_str();

                       child3D->ActWithPara(name,"JZRotate","Yaw",-m_azimuth);
                       //方位角
                       child3D->ActWithPara(name,"JTRotate","Pitch",-m_elevation);
                     //  qDebug() << m_azimuth << m_azimuth;

                   }
                }
            }
        }
    }

}

void IMainWidget::scenarioPropagated()
{

    /*
    ////Test
    double startTime = ssimCore->getPropagatedScenario()->startTime();
    double endTime = ssimCore->getPropagatedScenario()->endTime();
    QList<double>  res =     getEARS("嵩明1.2米自适应成像观测系统","X37B",startTime,endTime,10);

    double elevationAngleT = 5;//俯仰角阈值，deg
    double azimuthAngleT  = 360;//方位角阈值，deg
    double rangeT = 50000000;//斜距阈值，KM

    qDebug()<<"scenarioPropagated";


    int num = res.size()/4;
    for (int i = 0;i<num;i++)
    {

        int visble = 0;
        if (res[i*4+1]>elevationAngleT&&res[i*4+2]<=azimuthAngleT&&res[i*4+3]<=rangeT)
        {
            visble = 1;
        }
    }
    */

}

void IMainWidget::initializeGLisDone()
{
    ssimCore->openScenario("../user_data/502hx/502hx.sim");
    setActive3DWidgetViewPoint(1);

    // 加载地形
    FILE *fp = NULL;
    fp = fopen("../Ssim-data/data/terrainPath.inf","r");
    if (fp == NULL)
    {
       return ;
    }

    char image[200];
    char dem[200];
    char image_Mars[200];
    char dem_Mars[200];
    if(fp)
    {
       fscanf(fp,"%s",image);
       fscanf(fp,"%s",dem);
       fscanf(fp,"%s",image_Mars);
       fscanf(fp,"%s",dem_Mars);
       fclose(fp);
    }

    //m_3DView->createTerrainGeometry(SSIM_SOLAR_SYSTEM->lookup(SSIM_EARTH),image,dem);
    //m_3DView->createTerrainGeometry(SSIM_SOLAR_SYSTEM->lookup(SSIM_MARS),image_Mars,dem_Mars);

    //testCamera();// 吕亮改
}

bool IMainWidget::trackInfoVisibility() const
{

    return m_trackInfoVisible;
}

void IMainWidget::setTrackInfoVisibility(bool vis)
{

    m_trackInfoVisible = vis;
}

bool IMainWidget::mediaVisibility() const
{

    return m_mediaVisible;
}

void IMainWidget::setMediaVisibility(bool vis)
{
    m_mediaVisible = vis;
}

void IMainWidget::setActive3DWidgetViewPoint(int index)
{
    ////获得视点个数
    if(!ssimCore->scenario())
        return;
    int viewPointNum = ssimCore->scenario()->ViewPoint().size();

    if(m_curUrlIndex >= viewPointNum-1)
        m_curUrlIndex = viewPointNum-1;
    if(m_curUrlIndex < 0)
        m_curUrlIndex = 0;

    ////@ 获得
    for(int i = 0;i<ssimCore->getViewList().size();i++)
    {

       SSim3DView *view = dynamic_cast<SSim3DView *>(ssimCore->getViewList().at(i));
       if (view!=NULL)
       {
            ssimCore->setViewPoint(view,m_curUrlIndex);
       }
    }
}
