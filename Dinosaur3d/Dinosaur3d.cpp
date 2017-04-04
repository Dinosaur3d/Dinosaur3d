#include "Dinosaur3d.h"

namespace
{

class RendererImpl : public QQuickFramebufferObject::Renderer
{
    // Frame-buffer based implementation of osgViewer::GraphicsWindow
    class FBWindow : public osgViewer::GraphicsWindowEmbedded
    {
    public:
        explicit FBWindow(const RendererImpl* renderer)
            : osgViewer::GraphicsWindowEmbedded(0, 0, 0, 0), renderer_(*renderer)
        {
        }

        virtual bool makeCurrentImplementation()
        {
            assert ( renderer_.framebufferObject() != NULL );
            return renderer_.framebufferObject()->bind();
        }

        virtual bool releaseContextImplementation()
        {
            assert ( renderer_.framebufferObject() != NULL );
            return renderer_.framebufferObject()->release();
        }

    private:
        const RendererImpl& renderer_;
    };

public:
    explicit RendererImpl(const Dinosaur3d* item3d)
        : item3d_(*item3d)
    {
        if ( item3d_.model().get() )
            init();
    }

    void render()
    {
        if ( viewer_.get() )
        {
            // Without this line the model is not displayed in the second
            // and subsequent frames.
            QOpenGLContext::currentContext()->functions()->glUseProgram(0);

            if ( sizeHasChanged() )
            {
                updateSize();
            }

            viewer_->frame();
        }
    }

    const Dinosaur3d& item3d() const
    {
        return item3d_;
    }

    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size)
    {
        QOpenGLFramebufferObjectFormat format;
        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        format.setSamples(4);
        return new QOpenGLFramebufferObject(size, format);
    }

    void synchronize(QQuickFramebufferObject* item)
    {
        assert(item == &item3d_);

        Dinosaur3d& osgItem = static_cast<Dinosaur3d&>(*item);

        if ( item3d_.model() && !viewer_ )
        {
            init();
        }

        if ( viewer_.get() )
        {
            if ( viewer_->getSceneData() != osgItem.model() )
            {
                viewer_->getEventQueue()->clear();
                viewer_->setSceneData(osgItem.model().get());
                viewer_->home();
            }
            else
            {
                osgGA::EventQueue::Events events;
                osgItem.pendingEvents().takeEvents(events);
                viewer_->getEventQueue()->appendEvents(events);
            }
        }
    }

private: // functions

    void init()
    {
        viewer_ = new osgViewer::Viewer;
        viewer_->setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
        viewer_->setSceneData(item3d().model().get());
        viewer_->setCameraManipulator(new osgGA::TrackballManipulator);
        viewer_->addEventHandler(new osgViewer::StatsHandler);
        viewer_->addEventHandler(new osgViewer::HelpHandler);

        window_ = new FBWindow(this);
        updateSize();

        osg::Camera* camera = viewer_->getCamera();
        camera->setGraphicsContext( window_.get() );

        camera->setClearColor( osg::Vec4(0.2, 0.2, 0.6, 1.0) );
    }

    bool sizeHasChanged() const
    {
        const osg::GraphicsContext::Traits& t = *window_->getTraits();
        return t.width != item3d().width() || t.height != item3d().height();
    }

    void updateSize()
    {
        window_->resized(0, 0, item3d().width(), item3d().height());
        osg::Camera& camera = *viewer_->getCamera();
        const osg::GraphicsContext::Traits& t = *window_->getTraits();
        camera.setViewport( new osg::Viewport(0, 0, t.width, t.height) );
        const double aspectRatio = static_cast<double>(t.width)/static_cast<double>(t.height);
        camera.setProjectionMatrixAsPerspective(30.0f, aspectRatio, 1.0f, 10000.0f );
    }

private: // data
    const Dinosaur3d& item3d_;
    osg::ref_ptr<osgViewer::Viewer> viewer_;
    osg::observer_ptr<osgViewer::GraphicsWindow> window_;
};

int button(const QMouseEvent& event)
{
    switch ( event.button() )
    {
        case Qt::LeftButton:  return 1;
        case Qt::MidButton:   return 2;
        case Qt::RightButton: return 3;
        default:              return 0;
    }
}

} // unnamed namespace

////////////////////////////////////////////////////////////////////////////////
//                                 Dinosaur3d                                    //
////////////////////////////////////////////////////////////////////////////////

Dinosaur3d::Dinosaur3d(osg::ref_ptr<osg::Node> model)
    : model_(model), pendingEvents_(new PendingEvents)
{
    setAcceptedMouseButtons(Qt::AllButtons);
}

osg::ref_ptr<osg::Node> Dinosaur3d::model() const
{
    return model_;
}

void Dinosaur3d::setModel(osg::ref_ptr<osg::Node> model)
{
    model_ = model;
    update();
    emit modelChanged();
}

Dinosaur3d::PendingEvents& Dinosaur3d::pendingEvents()
{
    return *pendingEvents_;
}

QString Dinosaur3d::modelPath() const
{
    return modelPath_;
}

void Dinosaur3d::setModelPath(QString modelPath)
{
    if(modelPath.left(8) == "file:///")
    {
        modelPath = modelPath.right( modelPath.length()-8);
    }
    modelPath_ = modelPath;
    emit modelPathChanged();
    setModel(osgDB::readNodeFile(modelPath_.toStdString()));
}

QQuickFramebufferObject::Renderer* Dinosaur3d::createRenderer() const
{
    return new RendererImpl(this);
}

QSGNode* Dinosaur3d::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* upnData)
{
    QSGNode* node = QQuickFramebufferObject::updatePaintNode(oldNode, upnData);
    QSGSimpleTextureNode& textureNode = dynamic_cast<QSGSimpleTextureNode&>(*node);
    textureNode.setTextureCoordinatesTransform(QSGSimpleTextureNode::MirrorVertically);
    return node;
}

void Dinosaur3d::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry)
{
    pendingEvents_->windowResize(newGeometry.x(), newGeometry.y(), newGeometry.width(), newGeometry.height());
    QQuickFramebufferObject::geometryChanged(newGeometry, oldGeometry);
}

void Dinosaur3d::mousePressEvent(QMouseEvent *event)
{
    pendingEvents_->mouseButtonPress(event->x(), event->y(), button(*event));

    update();
}

void Dinosaur3d::mouseMoveEvent(QMouseEvent *event)
{
    pendingEvents_->mouseMotion(event->x(), event->y());

    update();
}

void Dinosaur3d::mouseReleaseEvent(QMouseEvent *event)
{
    pendingEvents_->mouseButtonRelease(event->x(), event->y(), button(*event));

    update();
}

void Dinosaur3d::mouseDoubleClickEvent(QMouseEvent *event)
{
    pendingEvents_->mouseDoubleButtonPress(event->x(), event->y(), button(*event));

    update();
}

void Dinosaur3d::wheelEvent(QWheelEvent* event)
{
    if ( event->delta() > 0 )
        pendingEvents_->mouseScroll(osgGA::GUIEventAdapter::SCROLL_UP);
    else
        pendingEvents_->mouseScroll(osgGA::GUIEventAdapter::SCROLL_DOWN);

    update();
}

void Dinosaur3d::keyPressEvent(QKeyEvent* event)
{
    const std::string text = event->text().toStdString();
    if ( text.size() == 1 )
    {
        pendingEvents_->keyPress(text[0]);
        update();
    }
}
