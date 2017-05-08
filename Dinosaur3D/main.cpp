#include "Dinosaur3dWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dinosaur3dWindow w;
    w.show();

    return a.exec();
}
