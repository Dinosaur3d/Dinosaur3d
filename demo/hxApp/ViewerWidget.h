#ifndef WIEWERWIDGET_H
#define WIEWERWIDGET_H
#include <QtWidgets/QWidget>
#include <QObject>
#include <osgViewer/CompositeViewer>
#include <osgQt/GraphicsWindowQt>
#include <osgGA/TrackballManipulator>
#include <QTimer>
#include <string>
#include <osg/ShapeDrawable>
#include <osg/ref_ptr>
#include <osg/MatrixTransform>

#include <pickhandler.h>
#include <travelmanipulator.h>

#include <hx/hx_che.h>
#include <hx/HxcManipulator.h>


class ViewerWidget2 : public QWidget,public osgViewer::CompositeViewer
{
    Q_OBJECT
public:
	ViewerWidget2(osgViewer::ViewerBase::ThreadingModel threadingModel=osgViewer::ViewerBase::SingleThreaded);
    ~ViewerWidget2();

	QWidget* addViewWidget( osgQt::GraphicsWindowQt* gw );

	osgQt::GraphicsWindowQt* createGraphicsWindow( int x, int y, int w, int h, const std::string& name="", bool windowDecoration=false );

	virtual void paintEvent( QPaintEvent* event );

    void setScene(osg::Group* root);

	void init();

	bool saveScreenShot(std::string path);

    osg::Camera* getCamera();

    inline PickHandler * getStonePickHandle(){return this->myPickHandler;}

protected:

	QTimer _timer;
//    osgViewer::View* _viewer;
    osg::ref_ptr<osgViewer::View> _viewer;

signals:
    void on_mouse_event(QEvent*);
    void on_key_event(QEvent*);
    // QObject interface
public:
    bool eventFilter(QObject *, QEvent *);

    void chooseModelScale(const float scale_x,const float scale_y,const float scale_z);
    void chooseModelRotate(const float &degree,const int &r_x,const int &r_y,const int &r_z);
    void chooseModelTranslate(const float &t_x,const float &t_y,const float &t_z);
    void chooseModelSetPos(const float &p_x,const float &p_y,const float &p_z);
    void chooseModelGetPos(float &p_x,float &p_y,float &p_z,bool &is_choose);
    void deleteStone();
    bool saveSingleStone();
    void produceStoneFile(unsigned int num_big, unsigned int num_mid, unsigned int num_sma);

    void printCameraPara();
    //以下相机控制函数
//    void setCameraView(const QString &arg1);
    void setVehicleAndCamera(const double& x,const double& y,const double&a,const int &camara_index);

    double getVehicleX();
    double getVehicleY();
    MyPosRotate getVehiclePosRot();
    MyPosRotate getNavigation1();
    MyPosRotate getNavigation2();
    MyPosRotate getAvoid1();
    MyPosRotate getAvoid2();
    MyPosRotate getBackwardAvoid1();
    MyPosRotate getBackwardAvoid2();
//    void setNavigation(MyCamaraAndVehicle &camapara,const int &index);
    // QWidget interface
protected:
    void mouseReleaseEvent(QMouseEvent *);

private:
    PickHandler *myPickHandler;
    HxcManipulator* _myManipulator;
    TravelManipulator *myTMP;

     osg::ref_ptr<osgGA::TrackballManipulator> tm;

public:
     void setCameraMode(int i);

};

#endif
