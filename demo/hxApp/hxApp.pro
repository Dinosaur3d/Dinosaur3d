#-------------------------------------------------
#
# Project created by QtCreator 2016-10-26T22:00:34
#
#-------------------------------------------------

QT += core network opengl widgets gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hxApp
TEMPLATE = app

include(../setting.pri)
include(../Inspace/SSim_Inc/inspace.pri)

QMAKE_CXXFLAGS_RELEASE += $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE += $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

CONFIG(debug, debug|release){
    LIBS += -lOpenThreadsd \
        -losgd \
        -losgDBd \
        -losgUtild \
        -losgGAd \
        -losgViewerd \
        -losgTextd \
        -losgWidgetd \
        -losgFXd \
        -losgQtd \
        -losgTerraind\
        -losgShadowd\
        -lhxd
}else{
    LIBS += -losg \
        -losgDB \
        -losgUtil \
        -losgGA \
        -losgViewer \
        -losgText \
        -losgWidget \
        -losgQt \
        -lOpenThreads \
        -losgFX \
        -losgTerrain \
        -losgShadow \
        -lhx
}

SOURCES += main.cpp\
        mainwindow.cpp \
        ViewerWidget.cpp \
        CaptureDrawCallback.cpp \
        pickhandler.cpp \
        travelmanipulator.cpp \
        IMainWidget.cpp \
		configdialog.cpp \
    network/tcpserver.cpp \
    network/tcpsocket.cpp \
    network/server.cpp

HEADERS  += mainwindow.h \
        ViewerWidget.h \
        CaptureDrawCallback.h \
        pickhandler.h \
        travelmanipulator.h \
        IMainWidget.h \
		configdialog.h \
    network/tcpserver.h \
    network/tcpsocket.h \
    network/server.h

FORMS    += mainwindow.ui \
	configdialog.ui \
    network/server.ui

DISTFILES +=
