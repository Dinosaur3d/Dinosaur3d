#ifndef DINOSAUR3DWINDOW_H
#define DINOSAUR3DWINDOW_H

#include <QMainWindow>

namespace Ui {
class Dinosaur3dWindow;
}

class Dinosaur3dWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit Dinosaur3dWindow(QWidget *parent = 0);
    ~Dinosaur3dWindow();

private:
    Ui::Dinosaur3dWindow *ui;
};

#endif // DINOSAUR3DWINDOW_H
