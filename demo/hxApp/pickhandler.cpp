#include "pickhandler.h"
#include <hx/hx_stone.h>
#include <hx/hx_terrain.h>
#include <hx/hx_options.h>
#include <QDebug>

PickHandler::PickHandler():m_x(0.0),m_y(0.0),zoom_count(0)
{
    m_choose=false;
    _pickCallback = 0;
}

bool PickHandler::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    osg::ref_ptr<osgViewer::View> view = dynamic_cast<osgViewer::View*>(&aa);
    if (!view) return false;

    switch (ea.getEventType())
    {
        //鼠标按下
    case(osgGA::GUIEventAdapter::PUSH) :
    {
        //更新鼠标位置
        m_x = ea.getX();
        m_y = ea.getY();
//        pick(view.get(), ea.getX(), ea.getY());
        break;
    }
    case(osgGA::GUIEventAdapter::RELEASE) :
    {
        if (m_x == ea.getX() && m_y == ea.getY())
        {
            //执行对象选取
            pick(view.get(), ea.getX(), ea.getY());
        }
        break;
    }
    default:
        break;
    }
    return false;
}

void PickHandler::pick(osg::ref_ptr<osgViewer::View> view, float x, float y)
{
//    osg::ref_ptr<osg::Node> node = new osg::Node();
//    osg::ref_ptr<osg::Group> parent = new osg::Group();
    //创建一个线段交集检测函数
    osgUtil::LineSegmentIntersector::Intersections intersections;
    if (view->computeIntersections(x, y, intersections))
    {
        osgUtil::LineSegmentIntersector::Intersection intersection = *intersections.begin();
        osg::NodePath& nodePath = intersection.nodePath;
        if(!parent.get() && !scribe.get() && !node.get())
        {
            node=new osg::Node;
            parent=new osg::Group;
            scribe=new osgFX::Scribe;
            g_node=new osg::Node;
        }
        node = (nodePath.size() >= 1) ? nodePath[nodePath.size() - 1] : 0;
        for(int i = nodePath.size()-1;i>=0;)//判断选中是否包含石头节点
        {
            osg::Node* m =nodePath[i];
           if(!m->getName().find("stone",0))
            {
                if(g_node!=node)
                {
                    m_choose=false;
                    if(parent->containsNode(scribe->asNode()))
                        parent->replaceChild(scribe.get(),g_node.get());
        //            node=new osg::Node;
                    parent=new osg::Group;
                    scribe=new osgFX::Scribe;
                    g_node=new osg::Node;
                }
                g_node=node;
                if(!m_choose)
                {
                    parent = (nodePath.size() >= 2) ? dynamic_cast<osg::Group*>(nodePath[nodePath.size() - 2]) : 0;
                    if(!scribe->containsNode(node.get()))
                        scribe->addChild(node.get());
                    parent->replaceChild(node.get(),scribe.get());
                    if(_pickCallback){
                        _pickCallback->pick(parent);
                    }
                    m_choose=true;
                }else {
                    parent->replaceChild(scribe.get(),node.get());
                    m_choose=false;
                }
               break;
            }
            else
               i-- ;
        }
        //得到选择的物体
//        if(zoom_count!=0)
//        {
//            osg::ref_ptr<osg::Node> tempNode = (nodePath.size() >= 1) ? nodePath[nodePath.size() - 1] : 0;
//            if(node.get()==tempNode.get())
//                parent = (nodePath.size() >= 3) ? dynamic_cast<osg::Group*>(nodePath[nodePath.size() - 3]) : 0;
//            else {
//                mt=new osg::MatrixTransform;
//                matrix.makeIdentity();
//                node=tempNode.get();
//                parent = (nodePath.size() >= 2) ? dynamic_cast<osg::Group*>(nodePath[nodePath.size() - 2]) : 0;
//                mt->addChild(node.get());
//                zoom_count=0;
//                scribe = new osgFX::Scribe();
//            }
//        }else {
//                node = (nodePath.size() >= 1) ? nodePath[nodePath.size() - 1] : 0;
//                parent = (nodePath.size() >= 2) ? dynamic_cast<osg::Group*>(nodePath[nodePath.size() - 2]) : 0;
//                if(!mt.get())
//                    mt=new osg::MatrixTransform;
//                mt->addChild(node.get());
//                scribe = new osgFX::Scribe();
//            }
    }

//    //用一种高亮显示来显示物体已经被选中
//    if (parent.get() && node.get())
//    {
//        osg::ref_ptr<osgFX::Scribe> parentAsScribe = dynamic_cast<osgFX::Scribe*>(parent.get());
//        if (!parentAsScribe)
//        {
//            m_choose=true;
//            //如果对象选择到，高亮显示
//            scribe->addChild(mt.get());
//            if(zoom_count!=0)
//                parent->replaceChild(mt.get(),scribe.get());
//            else {
//                parent->replaceChild(node.get(), scribe.get());
//                zoom_count++;
//            }
//        }
//        else
//        {
//            m_choose=false;
//            //如果没有没有选择到，则移除高亮显示的对象
//            osg::Node::ParentList parentList = parentAsScribe->getParents();
//            for (osg::Node::ParentList::iterator itr = parentList.begin();
//                itr != parentList.end();
//                ++itr)
//            {
//                (*itr)->replaceChild(parentAsScribe.get(), mt.get());
//            }
//        }
//    }
}

osg::ref_ptr<osg::Node> PickHandler::getChooseNode()
{
    if(node.get())
        return node.get();
    return nullptr;
}

osg::ref_ptr<osg::Group> PickHandler::getChooseGroup()
{
    if(parent.get())
        return parent.get();
    return nullptr;
}

void PickHandler::scale(const float &scale_x, const float &scale_y, const float &scale_z)
{
    if(!m_choose)
        return;
    ++zoom_count;
    mt=dynamic_cast<osg::MatrixTransform*>(parent.get());
//    mt->postMult(osg::Matrix::scale(scale_x,scale_y,scale_z));
    mt->preMult(osg::Matrix::scale(scale_x,scale_y,scale_z));
    //    mt->postMult(osg::Matrix::scale(scale_x,scale_y,scale_z));
}

void PickHandler::rotate(const float &angle, const int &rotate_x, const int &rotate_y, const int &rotate_z)
{
    if(!m_choose)
        return;
    mt=dynamic_cast<osg::MatrixTransform*>(parent.get());

    osg::Matrix r_matrix=mt->getMatrix();
    osg::Matrix inverse_matrix=osg::Matrix::inverse(r_matrix);
    osg::Vec3 vec(rotate_x,rotate_y,rotate_z);
    osg::Vec3 vec_o(0.0,0.0,0.0);
    vec=vec*inverse_matrix;
    vec_o=vec_o*inverse_matrix;
    vec-=vec_o;
    mt->preMult(osg::Matrix::rotate(osg::DegreesToRadians(angle),vec[0],vec[1],vec[2]));
}

void PickHandler::translate(const float &tran_x, const float &tran_y, const float &tran_z)
{
    if(!m_choose)
        return;
    mt=dynamic_cast<osg::MatrixTransform*>(parent.get());
    mt->postMult(osg::Matrix::translate(tran_x,tran_y,tran_z));
    _pickCallback->pick(parent);
}

void PickHandler::setPos(const float &pos_x, const float &pos_y, const float &pos_z)
{
    if(!m_choose)
        return;
    mt=dynamic_cast<osg::MatrixTransform*>(parent.get());
    osg::Matrix t_matrix=mt->getMatrix();
    t_matrix.setTrans(pos_x,pos_y,pos_z);
    mt->setMatrix(t_matrix);
}

void PickHandler::deleteStone()
{
    if(!m_choose)
        return;
    osg::ref_ptr<osg::Group> pp=parent->getParent(0);
    pp->removeChild(parent.get());
    parent=nullptr;
    mt=nullptr;
    node=nullptr;
    g_node=nullptr;
    scribe=nullptr;
    m_choose=false;
}

void PickHandler::getPos(float &pos_x, float &pos_y, float &pos_z, bool &is_choose)
{
    is_choose=m_choose;
    if(!m_choose)
        return;
    mt=dynamic_cast<osg::MatrixTransform*>(parent.get());
    osg::Matrix t_matrix=mt->getMatrix();
    osg::Vec3 pos_v=t_matrix.getTrans();
    pos_x=pos_v.x();
    pos_y=pos_v.y();
    pos_z=pos_v.z();
}

bool PickHandler::saveStone()
{
    if(matrix.isIdentity())
        return true;
    bool tem=hx_stone::intance()->saveSingleStoneState(parent,matrix);
    if(tem)
        return true;
    else
        return false;
}

