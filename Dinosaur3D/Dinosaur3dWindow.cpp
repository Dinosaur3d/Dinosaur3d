#include "Dinosaur3dWindow.h"
#include "ui_Dinosaur3dWindow.h"

Dinosaur3dWindow::Dinosaur3dWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Dinosaur3dWindow)
{
    ui->setupUi(this);
}

Dinosaur3dWindow::~Dinosaur3dWindow()
{
    delete ui;
}
