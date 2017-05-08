#include <GL/glew.h>
#include "ViewerWidget.h"
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>

#include <QHBoxLayout>
#include <QGridLayout>
#include <osgDB/WriteFile>
#include <osg/ShapeDrawable>
#include <osg/LightModel>
#include <osg/Fog>

#include "CaptureDrawCallback.h"
#include <QMessageBox>
#include <osg/ColorMatrix>

#include <osgFX/Registry>
#include <osgFX/Effect>
#include <osgFX/SpecularHighlights>

#include <osg/MatrixTransform>
#include <osg/Hint>
#include <osgGA/GUIEventHandler>

#include <qdebug.h>

#include <QFile>
#include <QTextStream>


#include <osgTerrain/Terrain>

#include <hx/hx_options.h>
#include <hx/hx_terrain.h>
#include <hx/hx_stone.h>
#include <hx/sim_core.h>

#include <QDebug>
#include "travelmanipulator.h"
#include <hx/HxcManipulator.h>


bool ViewerWidget2::saveScreenShot( std::string path )
{
	static CaptureDrawCallback* ccb = new CaptureDrawCallback();
	ccb->_savepath = path;
	if (!_viewer)
	{
		return false;
	}
	_viewer->getCamera()->setPostDrawCallback(ccb);	


    return true;
}

osg::Camera *ViewerWidget2::getCamera(){
    if(!_viewer){
        return 0;
    }
    return _viewer->getCamera();
}

bool ViewerWidget2::eventFilter(QObject *o, QEvent *e)
{
    switch (e->type()) {
    case QEvent::MouseButtonPress:
        emit on_mouse_event(e);
    case QEvent::MouseMove:
        emit on_mouse_event(e);
        break;
    case QEvent::KeyPress:
        emit on_key_event(e);
        break;
    default:
        break;
    }
    return false;
}

void ViewerWidget2::chooseModelScale(const float scale_x, const float scale_y, const float scale_z)
{
    myPickHandler->scale(scale_x,scale_y,scale_z);
}

void ViewerWidget2::chooseModelRotate(const float &degree, const int &r_x, const int &r_y, const int &r_z)
{
    myPickHandler->rotate(degree,r_x,r_y,r_z);
}

void ViewerWidget2::chooseModelTranslate(const float &t_x, const float &t_y, const float &t_z)
{
    myPickHandler->translate(t_x,t_y,t_z);
}

void ViewerWidget2::chooseModelSetPos(const float &p_x, const float &p_y, const float &p_z)
{
    float c_z=hx_terrain::intance()->get_height(p_x,p_y);
    myPickHandler->setPos(p_x,p_y,c_z);
}

void ViewerWidget2::chooseModelGetPos(float &p_x, float &p_y, float &p_z, bool &is_choose)
{
    myPickHandler->getPos(p_x,p_y,p_z,is_choose);
}

void ViewerWidget2::deleteStone()
{
    myPickHandler->deleteStone();
    unsigned int big_count=hx_stone::intance()->getBigCount();
    --big_count;
    unsigned int mid_count=hx_stone::intance()->getMiddleCount();
    --mid_count;
    unsigned int small_count=hx_stone::intance()->getSmallCount();
    --small_count;
    hx_stone::intance()->setMiddleCount(mid_count);
}

bool ViewerWidget2::saveSingleStone()
{
    if(myPickHandler->saveStone())
        return true;
    else
        return false;
}

void ViewerWidget2::produceStoneFile(unsigned int num_big, unsigned int num_mid, unsigned int num_sma)
{
    hx_stone::intance()->produceStone(num_big,num_mid,num_sma);
//    hx_stone::intance()->init();
}

void ViewerWidget2::printCameraPara()
{

    osg::Vec3 position1(0.0,0.0,0.0);
    osg::Vec3 rotate1(0.0,0.0,0.0);
    position1= myTMP->GetPosition();
    rotate1=myTMP->GetRotate();
    qDebug()<<position1[0]<<"    "<<position1[1]<<"    "<<position1[2];
    qDebug()<<rotate1[0]<<"    "<<rotate1[1]<<"    "<<rotate1[2];

}

void ViewerWidget2::setVehicleAndCamera(const double &x, const double &y, const double &a, const int &camara_index)
{
    double o_x=hx_che::intance()->getVehiclePosX();
    double o_y=hx_che::intance()->getVehiclePosY();
    if(camara_index==0)
    {
        o_x-=y*sin(a);
        o_y-=y*cos(a);
    }
    else {
        o_x+=x;
        o_y+=y;
    }
    hx_che::intance()->move(o_x,o_y,a);
    if(camara_index==0)
    { 
        if(tm==NULL)
        {
            myTMP=NULL;
            tm = new osgGA::TrackballManipulator;
            _viewer->setCameraManipulator(tm ,true);
        }
    }
    else if (camara_index==1) {
        //导航相机1
        tm=NULL;
        if(myTMP==NULL)
        {
            myTMP=new TravelManipulator;
            _viewer->setCameraManipulator(myTMP);
        }
//            myTMP->setByMatrix(hx_che::intance()->getNavigation1Matrix());
        float navigation1_h=hx_terrain::intance()->get_height(o_x,o_y);
        osg::Vec3 navigation1_pos(o_x,o_y,navigation1_h);

        osg::Quat q_rot,q_so;
        osg::Vec3 trans,scale;
        hx_che::intance()->getNavigation1Matrix().decompose(trans,q_rot,scale,q_so);
        double roll,pitch,yaw=0.0;
        myTMP->quatToHPR(q_rot,yaw,pitch,roll);
        yaw=hx_che::intance()->getVehicleRotA();
        osg::Vec3 navigation1_rot(osg::PI_2-pitch,-roll,yaw/180.0*osg::PI);

        osg::Matrix tempMat;
        tempMat.postMult(osg::Matrix::translate(0.0,0.0,2.05));
        tempMat.postMult(osg::Matrix::rotate(osg::DegreesToRadians(yaw),0,0,1));
        tempMat.postMult(osg::Matrix::rotate(roll,0,1,0));
        tempMat.postMult(osg::Matrix::rotate(pitch,1,0,0));
        tempMat.decompose(trans,q_rot,scale,q_so);
        qDebug()<<trans[0]<<"    "<<trans[1]<<"    "<<trans[2];
        qDebug()<<navigation1_rot[0]<<"    "<<navigation1_rot[1]<<"    "<<navigation1_rot[2];
        navigation1_pos=osg::Vec3(o_x+trans[0],o_y+1+trans[1],navigation1_h+trans[2]);

        myTMP->SetPosition(navigation1_pos);
        myTMP->SetRotate(navigation1_rot);
    }
    else if (camara_index==2) {
        //导航相机2

    }
    else if (camara_index==3) {

    }
    else if (camara_index==4) {

    }
}

double ViewerWidget2::getVehicleX()
{
    return hx_che::intance()->getVehiclePosX();
}

double ViewerWidget2::getVehicleY()
{
    return hx_che::intance()->getVehiclePosY();
}

MyPosRotate ViewerWidget2::getVehiclePosRot()
{
    osg::Matrixd vehicle=hx_che::intance()->getVehicleMatrix();
    MyPosRotate vehiclePosRat;
    vehiclePosRat.position=vehicle.getTrans();
//    osg::Quat rot=vehicle.getRotate();
    osg::Quat q_rot,q_so;
    osg::Vec3 trans,scale;
    hx_che::intance()->getNavigation1Matrix().decompose(trans,q_rot,scale,q_so);
    double roll,pitch,yaw=0.0;
    myTMP->quatToHPR(q_rot,yaw,pitch,roll);
    pitch=pitch/(osg::PI)*180.0;
    roll=roll/(osg::PI)*180.0;
    yaw=yaw/(osg::PI)*180.0;
    vehiclePosRat.rotate._v[0]=pitch;
    vehiclePosRat.rotate._v[1]=roll;
    int mm=hx_che::intance()->getVehicleRotA();
    mm = (mm+360)%360;
    vehiclePosRat.rotate._v[2]=mm;
    return vehiclePosRat;
}

MyPosRotate ViewerWidget2::getNavigation1()
{
//    osg::Matrixd navigation1=hx_che::intance()->getNavigation1Matrix();
    MyPosRotate navigation1PosRat;
//    navigation1PosRat.position=navigation1.getTrans();

    osg::Quat q_rot,q_so;
    osg::Vec3 trans,scale;
    hx_che::intance()->getNavigation1Matrix().decompose(trans,q_rot,scale,q_so);
    navigation1PosRat.position=trans;
//    osg::Quat rot=navigation1.getRotate();
    double roll,pitch,yaw=0.0;
    myTMP->quatToHPR(q_rot,yaw,pitch,roll);
    pitch=pitch/(osg::PI)*180.0;
    roll=roll/(osg::PI)*180.0;
    yaw=yaw/(osg::PI)*180.0;
    navigation1PosRat.rotate[0]=pitch;
    navigation1PosRat.rotate._v[1]=roll;
    navigation1PosRat.rotate._v[2]=hx_che::intance()->getVehicleRotA();
    return navigation1PosRat;
}

MyPosRotate ViewerWidget2::getNavigation2()
{
    osg::Matrixd navigation2=hx_che::intance()->getNavigation2Matrix();
    MyPosRotate navigation2PosRat;
    navigation2PosRat.position=navigation2.getTrans();
    osg::Quat q_rot,q_so;
    osg::Vec3 trans,scale;
    hx_che::intance()->getNavigation2Matrix().decompose(trans,q_rot,scale,q_so);

    double roll,pitch,yaw=0.0;
    myTMP->quatToHPR(q_rot,yaw,pitch,roll);
    pitch=pitch/(osg::PI)*180.0;
    roll=roll/(osg::PI)*180.0;
    yaw=yaw/(osg::PI)*180.0;
    navigation2PosRat.rotate._v[0]=pitch;
    navigation2PosRat.rotate._v[1]=roll;
    navigation2PosRat.rotate._v[2]=hx_che::intance()->getVehicleRotA();
    return navigation2PosRat;
}

MyPosRotate ViewerWidget2::getAvoid1()
{
    osg::Matrixd avoid1=hx_che::intance()->getAvoid1Matrix();
    MyPosRotate avoid1PosRat;
    avoid1PosRat.position=avoid1.getTrans();
    osg::Quat q_rot,q_so;
    osg::Vec3 trans,scale;
    hx_che::intance()->getAvoid1Matrix().decompose(trans,q_rot,scale,q_so);

    double roll,pitch,yaw=0.0;
    myTMP->quatToHPR(q_rot,yaw,pitch,roll);
    pitch=pitch/(osg::PI)*180.0;
    roll=roll/(osg::PI)*180.0;
    yaw=180-yaw/(osg::PI)*180.0;
    avoid1PosRat.rotate._v[0]=pitch;
    avoid1PosRat.rotate._v[1]=roll;
    avoid1PosRat.rotate._v[2]=yaw;
    return avoid1PosRat;
}

MyPosRotate ViewerWidget2::getAvoid2()
{
    osg::Matrixd avoid2=hx_che::intance()->getAvoid2Matrix();
    MyPosRotate avoid2PosRat;
    avoid2PosRat.position=avoid2.getTrans();
    osg::Quat q_rot,q_so;
    osg::Vec3 trans,scale;
    hx_che::intance()->getAvoid2Matrix().decompose(trans,q_rot,scale,q_so);

    double roll,pitch,yaw=0.0;
    myTMP->quatToHPR(q_rot,yaw,pitch,roll);
    pitch=pitch/(osg::PI)*180.0;
    roll=roll/(osg::PI)*180.0;
    yaw=180-yaw/(osg::PI)*180.0;
    avoid2PosRat.rotate._v[0]=pitch;
    avoid2PosRat.rotate._v[1]=roll;
    avoid2PosRat.rotate._v[2]=yaw;
    return avoid2PosRat;
}

MyPosRotate ViewerWidget2::getBackwardAvoid1()
{
    osg::Matrixd back_avoid1=hx_che::intance()->getBackwardAvoid1Matrix();
    MyPosRotate back_avoid1PosRat;
    back_avoid1PosRat.position=back_avoid1.getTrans();
    osg::Quat q_rot,q_so;
    osg::Vec3 trans,scale;
    hx_che::intance()->getBackwardAvoid1Matrix().decompose(trans,q_rot,scale,q_so);

    double roll,pitch,yaw=0.0;
    myTMP->quatToHPR(q_rot,yaw,pitch,roll);
    pitch=pitch/(osg::PI)*180.0;
    roll=roll/(osg::PI)*180.0;
    yaw=180-yaw/(osg::PI)*180.0;
    back_avoid1PosRat.rotate._v[0]=pitch;
    back_avoid1PosRat.rotate._v[1]=roll;
    back_avoid1PosRat.rotate._v[2]=yaw;
    return back_avoid1PosRat;
}

MyPosRotate ViewerWidget2::getBackwardAvoid2()
{
    osg::Matrixd back_avoid2=hx_che::intance()->getBackwardAvoid2Matrix();
    MyPosRotate back_avoid2PosRat;
    back_avoid2PosRat.position=back_avoid2.getTrans();
    osg::Quat q_rot,q_so;
    osg::Vec3 trans,scale;
    hx_che::intance()->getBackwardAvoid2Matrix().decompose(trans,q_rot,scale,q_so);

    double roll,pitch,yaw=0.0;
    myTMP->quatToHPR(q_rot,yaw,pitch,roll);
    pitch=pitch/(osg::PI)*180.0;
    roll=roll/(osg::PI)*180.0;
    yaw=180-yaw/(osg::PI)*180.0;
    back_avoid2PosRat.rotate._v[0]=pitch;
    back_avoid2PosRat.rotate._v[1]=roll;
    back_avoid2PosRat.rotate._v[2]=yaw;
    return back_avoid2PosRat;
}

void ViewerWidget2::mouseReleaseEvent(QMouseEvent *)
{
    return;
}


ViewerWidget2::ViewerWidget2( osgViewer::ViewerBase::ThreadingModel threadingModel/*=osgViewer::ViewerBase::SingleThreaded*/ ) : QWidget()
{

	_viewer = 0;
    myPickHandler=new PickHandler;
    myTMP=new TravelManipulator;
    tm = new osgGA::TrackballManipulator;
    init();
    this->setMinimumSize(300,300);
}

ViewerWidget2::~ViewerWidget2()
{
    if(myPickHandler!=nullptr)
        delete myPickHandler;
    if(myTMP!=nullptr)
        delete myTMP;
}

QWidget* ViewerWidget2::addViewWidget( osgQt::GraphicsWindowQt* gw )
{
    addView(_viewer);
	osg::Camera* camera = _viewer->getCamera();
	camera->setGraphicsContext( gw );

	const osg::GraphicsContext::Traits* traits = gw->getTraits();
    camera->setClearMask(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	camera->setClearColor( osg::Vec4(0,0,0, 1.0) );
    camera->setClearStencil(0);
	camera->setViewport( new osg::Viewport(0, 0, traits->width, traits->height) );
    camera->setProjectionMatrixAsPerspective(60.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 0.001f, 5000.0f );

	return gw->getGLWidget();
}

osgQt::GraphicsWindowQt* ViewerWidget2::createGraphicsWindow( int x, int y, int w, int h, const std::string& name/*=""*/, bool windowDecoration/*=false */ )
{
	osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	traits->windowName = name;
	traits->windowDecoration = windowDecoration;
	traits->x = x;
	traits->y = y;
    traits->width = w;
    traits->height = h;
	traits->doubleBuffer = true;
	traits->alpha = ds->getMinimumNumAlphaBits();
	traits->stencil = ds->getMinimumNumStencilBits();
	traits->sampleBuffers = ds->getMultiSamples();
	traits->samples = ds->getNumMultiSamples();
    traits->sharedContext = 0;

	return new osgQt::GraphicsWindowQt(traits.get());
}
#include <hx/hx_terrain.h>

void ViewerWidget2::paintEvent( QPaintEvent* event )
{
    frame();
}


void ViewerWidget2::setScene(osg::Group *root)
{
    if(!_viewer){
        init();
    }
    _viewer->setSceneData(root);

    HxcManipulator* mainpu = new HxcManipulator();
    _viewer->setCameraManipulator(mainpu);
    mainpu->setNode(hx_che::intance()->get_node());
    mainpu->home(1);

    _viewer->addEventHandler(myPickHandler);
//    osgGA::TrackballManipulator * tm = new osgGA::TrackballManipulator;
//    _viewer->setCameraManipulator( tm ,true);
//    tm->setNode(hx_che::intance()->get_node());
//    tm->home(1);
}
#include <QEvent>

void ViewerWidget2::init()
{
	if (_viewer == 0)
	{
        _viewer = new osgViewer::View();
//        _viewer->setCameraManipulator(new osgGA::TrackballManipulator);
        setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
		// disable the default setting of viewer.done() by pressing Escape.
        setKeyEventSetsDone(0);


        QWidget* popupWidget = addViewWidget( createGraphicsWindow(0,0,this->width()-2,this->height()-2));
        QGridLayout *grid=new QGridLayout;
//		QHBoxLayout* grid = new QHBoxLayout();
        grid->setMargin(1);
        grid->setSpacing(1);
        popupWidget->installEventFilter(this);
		grid->addWidget(popupWidget);
		setLayout( grid );

		connect( &_timer, SIGNAL(timeout()), this, SLOT(update()) );
		_timer.start( 10 );
	}
}

void ViewerWidget2::setCameraMode(int i)
{
    if(!_viewer){
        return;
    }
    HxcManipulator* hmp = (HxcManipulator*)_viewer->getCameraManipulator();
    if(!hmp){
        return;
    }
    hmp->setManiMode(i);
    hmp->setNode(hx_che::intance()->get_node());
    hmp->home(1);

}
