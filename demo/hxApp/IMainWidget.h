#ifndef IMAINWIDGET_H
#define IMAINWIDGET_H

#include <QWidget>
////@包含SSIM SDK开发包头文件
#include <SSimMainApp/SSimHeader.h>

class SSimCore;
class SSim3DView;
class SSim2DView;
class IMainWidget : public QWidget
{
    Q_OBJECT

public:
    QWidget*                m_3DWidget;
    IMainWidget(QWidget *parent = 0);
    ~IMainWidget();
    Q_PROPERTY(bool mediaVisible READ mediaVisibility WRITE setMediaVisibility NOTIFY mediaVisibilityChanged);
    Q_PROPERTY(bool trackInfoVisible READ trackInfoVisibility WRITE setTrackInfoVisibility NOTIFY trackInfoVisibilityChanged);
    Q_PROPERTY(bool azimuth READ getAzimuth);

    bool                    mediaVisibility() const;
    void                    setMediaVisibility(bool vis);
    bool                    trackInfoVisibility() const;
    void                    setTrackInfoVisibility(bool vis);
    double                  getAzimuth(){return m_azimuth;}

    Q_INVOKABLE QString getSelectBodyName(){return m_selectBodyName;}
    Q_INVOKABLE QString getTargetName(){return m_targetName;}
    Q_INVOKABLE double getaTrackAzimuth(){return m_azimuth;}
    Q_INVOKABLE double getaTrackElevation(){return m_elevation;}
    Q_INVOKABLE double getaTrackRange(){return m_range;}

private:
    SSimCore*               ssimCore;
    SSim3DView*             m_3DView;
    ////@是否显示视频QML窗口
    bool                    m_mediaVisible;
    ////@是否显示跟踪信息QML窗口
    bool                    m_trackInfoVisible;

    //选中的目标的名称
    QString m_targetName;
    //选中的目标的名称
    QString m_selectBodyName;
    //方位角度、高度角、距离
    double m_azimuth,m_elevation,m_range;
    int                     m_curUrlIndex;

public slots:
    void timeChanged();
    void scenarioPropagated();
    void initializeGLisDone();

    void				setActive3DWidgetViewPoint(int index);

signals:

    /**
     * @brief mediaVisibilityChanged 是否显示视频QML窗口信号，QML可接收
     * @param mediaVisible true表示显示，false表示隐藏
     */
    void mediaVisibilityChanged(const bool &mediaVisible);

    /**
     * @brief trackInfoVisibilityChanged 是否显示视频QML窗口信号，QML可接收
     * @param mediaVisible true表示显示，false表示隐藏
     */
    void trackInfoVisibilityChanged(const bool &mediaVisible);

};

#endif // IMAINWIDGET_H
