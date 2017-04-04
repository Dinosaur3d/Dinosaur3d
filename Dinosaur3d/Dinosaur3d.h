#ifndef DINOSAUR3D_H
#define DINOSAUR3D_H

#include <QQuickFramebufferObject>
#include <QtQuick/QQuickWindow>
#include <qsgsimpletexturenode.h>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLFramebufferObjectFormat>
#include <QDebug>

#include <osg/Node>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>

#include <memory>
#include <cassert>

class Dinosaur3d : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Dinosaur3d)
    Q_PROPERTY(QString modelPath READ modelPath WRITE setModelPath NOTIFY modelPathChanged)

public:
    class PendingEvents : public osgGA::EventQueue
    {
    public:
        PendingEvents() : osgGA::EventQueue(osgGA::GUIEventAdapter::Y_INCREASING_DOWNWARDS) {}
    };

public:
    Dinosaur3d(osg::ref_ptr<osg::Node> model = osg::ref_ptr<osg::Node>());
    ~Dinosaur3d() {}

    Renderer *createRenderer() const;

    osg::ref_ptr<osg::Node> model() const;
    void setModel(osg::ref_ptr<osg::Node> model);

    PendingEvents& pendingEvents();

    QString modelPath() const;
    void setModelPath(QString modelPath);

signals:
    void modelChanged();
    void modelPathChanged();

private:
    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) Q_DECL_OVERRIDE;

    void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent* );
    void keyPressEvent(QKeyEvent*);

private:
    osg::ref_ptr<osg::Node> model_;
    const std::unique_ptr<PendingEvents> pendingEvents_;
    QString modelPath_;
};

#endif // DINOSAUR3D_H
