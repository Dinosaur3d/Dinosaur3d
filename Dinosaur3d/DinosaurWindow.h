#ifndef DINOSAURWINDOW_H
#define DINOSAURWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QApplication>
#include <QRect>
#include <QSize>
#include <QUrl>
#include <QQmlContext>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <qDebug>

namespace Ui {
class DinosaurWindow;
}

class DinosaurWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DinosaurWindow(QWidget *parent = 0);
    ~DinosaurWindow();

public slots:
    qreal getWindowX();
    qreal getWindowY();
    void setWindowPosition(int x, int y);
    void setWindowMin();
    void setWindowMax( bool max);
    void setWindowClose();
    void setWindowTray();
    void trayiconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    Ui::DinosaurWindow *ui;
    QSystemTrayIcon *m_trayIcon;
    QAction *m_quitAction;
    QMenu   *m_trayIconMenu;
};

#endif // DINOSAURWINDOW_H
