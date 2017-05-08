#ifndef PICKHANDLER_H
#define PICKHANDLER_H
#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/MatrixTransform>
#include <osg/Matrix>
#include <osgViewer/Viewer>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgFX/Scribe>

#include <osgGA/GUIEventHandler>

class PickCallback
{
public:
    virtual ~PickCallback(){}
    virtual void pick(osg::Node* node){}
};

class PickHandler : public osgGA::GUIEventHandler
{
public:
    PickHandler();

    // EventHandler interface
public:
    bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
    //对象选取时间处理器
    void pick(osg::ref_ptr<osgViewer::View> view, float x, float y);
    osg::ref_ptr<osg::Node>getChooseNode();
    osg::ref_ptr<osg::Group>getChooseGroup();
    void scale(const float &scale_x,const float &scale_y,const float &scale_z);
    void rotate(const float &angle,const int &rotate_x,const int &rotate_y,const int &rotate_z);
    void translate(const float &tran_x,const float &tran_y,const float &tran_z);
    void setPos(const float &pos_x,const float &pos_y,const float &pos_z);
    void deleteStone();

    void getPos(float &pos_x,float &pos_y,float& pos_z,bool &is_choose);

    bool saveStone();

    inline void setPickCallback(PickCallback* pcb){if(_pickCallback){delete _pickCallback;}_pickCallback=pcb;}

private:
    float m_x;
    float m_y;
    bool m_choose;
    int zoom_count;
    osg::ref_ptr<osg::Node>node;
    osg::ref_ptr<osg::Node>g_node;
    osg::ref_ptr<osg::Group>parent;
    osg::ref_ptr<osgFX::Scribe> scribe;
    osg::ref_ptr<osg::MatrixTransform> mt;
    osg::Matrix matrix;
    PickCallback* _pickCallback;
};

#endif // PICKHANDLER_H
