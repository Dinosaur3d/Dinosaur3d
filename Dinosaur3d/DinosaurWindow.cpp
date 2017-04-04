#include "DinosaurWindow.h"
#include "ui_DinosaurWindow.h"

DinosaurWindow::DinosaurWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DinosaurWindow)
{
    ui->setupUi(this);

    ui->quickWidget->setClearColor(QColor("transparent"));
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint| Qt::Window);

    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();
    this->setMinimumSize(QSize(1030, 630));
    this->setGeometry( (screenRect.width()-1000)/2, (screenRect.height()-600)/2-20, 1000, 600);

    ui->quickWidget->setSource(QUrl("qrc:/qml/LoginMain.qml"));
    ui->quickWidget->rootContext()->setContextProperty("mainApp", this);

    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setToolTip(QString::fromUtf8("Dinosaur3d"));
    QIcon icon = QIcon(":/image/image/login/head04.png");
    m_trayIcon->setIcon(icon);
    connect( m_trayIcon,&QSystemTrayIcon::activated, this, &DinosaurWindow::trayiconActivated);

    m_quitAction = new QAction(QString::fromUtf8("Quit"), this);
    m_trayIconMenu = new QMenu(this);
    m_trayIconMenu->addAction(m_quitAction);
    connect(m_quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    m_trayIcon->setContextMenu(m_trayIconMenu);
}

DinosaurWindow::~DinosaurWindow()
{
    delete ui;
}

qreal DinosaurWindow::getWindowX()
{
    return this->x();
}

qreal DinosaurWindow::getWindowY()
{
    return this->y();
}

void DinosaurWindow::setWindowPosition(int x, int y)
{
    this->move(x, y);
}

void DinosaurWindow::setWindowMin()
{
    this->showMinimized();
}

void DinosaurWindow::setWindowMax(bool max)
{
    if( max == false)
        this->showMaximized();
    else
        this->showNormal();
}

void DinosaurWindow::setWindowClose()
{
    this->close();
}

void DinosaurWindow::setWindowTray()
{
    m_trayIcon->show();
    m_trayIcon->showMessage( "Hi~", "小恐龙隐藏在托盘了喔~", QSystemTrayIcon::NoIcon, 1000);
    this->hide();
}

void DinosaurWindow::trayiconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
        case QSystemTrayIcon::Trigger:
            //break;
        case QSystemTrayIcon::DoubleClick:
            this->showNormal();
            break;
        default:
            break;
    }
}
