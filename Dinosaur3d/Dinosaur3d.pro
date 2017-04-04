#-------------------------------------------------
#
# Project created by QtCreator 2017-04-04T19:29:26
#
#-------------------------------------------------

QT       += core gui quickwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#################################
CONFIG(debug, debug|release){
    TARGET = Dinosaur3dd
}else{
    TARGET = Dinosaur3d
}
TEMPLATE = app

DISTFILES += *.qml

SOURCES += main.cpp \
    DinosaurWindow.cpp \
    Dinosaur3d.cpp \
    DinosaurNetTest.cpp

HEADERS  += \
    DinosaurWindow.h \
    Dinosaur3d.h \
    DinosaurNetTest.h

FORMS += \
    DinosaurWindow.ui

RESOURCES += \
    login.qrc

RC_FILE = ./icon/Icon.rc

#################################
DESTDIR = $$PWD/../../bin
DLLDESTDIR = $$PWD/../../bin

################# Lib ###################
OSGPATH = $$PWD/../../cdk/osg
OSG3RDPATH = $$OSGPATH/3rd

INCLUDEPATH += $$OSGPATH/include
INCLUDEPATH += $$OSG3RDPATH/include

LIBS += -L"$$OSGPATH/lib"
LIBS += -L"$$OSG3RDPATH/lib"

CONFIG(debug, debug|release){
    LIBS += -losgd \
        -lOpenThreadsd \
        -losgDBd \
        -losgGAd \
        -losgViewerd \
}else{
    LIBS += -losg \
        -lOpenThreads \
        -losgDB \
        -losgGA \
        -losgViewer \
}

