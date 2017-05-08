#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "IMainWidget.h"
#include <QMainWindow>
#include "ViewerWidget.h"
#include "pickhandler.h"
#include "network/server.h"
#include <configdialog.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void slot_open();
    void slot_screen();
    void slot_mouse(QEvent* e);
    void slot_KeyEvent(QEvent *e);

    void slot_set();

    void on_northBtn_clicked();

    void on_westBtn_clicked();

    void on_eastBtn_clicked();

    void on_highBtn_clicked();

    void on_sourthBtn_clicked();

    void on_lowBtn_clicked();

    void on_zoomOutBtn_clicked();

    void on_clockwiseBtn_clicked();

    void on_deleteBtn_clicked();

    void on_saveStoneBtn_clicked();

    void on_positionBtn_clicked();

    void on_produceBtn_clicked();

    //导航相机拍照
    void on_text_takepic_navi();
    //导航相机影像流输出
    void on_text_outpic_navi();
    //导航相机保存拍照影像
    void on_text_savepic_navi();

    //避障相机拍照
    void on_text_takepic_avoid();
    //void on_text_outpic_avoid();
    //避障相机保存拍照影像
    void on_text_savepic_avoid();

    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_light_clicked();
    void on_navi_a_clicked();

    void on_navi_b_clicked();

    void on_foravoid_a_clicked();

    void on_foravoid_b_clicked();

    void on_backavoid_a_clicked();

    void on_backavoid_b_clicked();

    void on_userOutImageBtn_clicked();

    void on_testBtn_clicked();
    void ConfigDialog_show();

    void on_actionServer_triggered();

public:
    void updatePickStone(osg::Node* n);

private:
    void initCameraContralWin();
    void initStoneWin();
    void initText();

private:
    Ui::MainWindow *ui;
    ViewerWidget2 * m_3DWidget;
    IMainWidget *m_inspaceWidget;
    server *m_server;
    ConfigDialog *dialog;
};

class StonePickCallback : public PickCallback
{
    // PickCallback interface

public:
    virtual ~StonePickCallback();
    void pick(osg::Node *node);
    MainWindow* mw;
};

#endif // MAINWINDOW_H
