#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog(QWidget *parent = 0);
    ~ConfigDialog();
    void Read_camera_data();
private slots:
    void Setbutton_clicked();
    void Reset_Camera_Data();

private:
    Ui::ConfigDialog *ui;
};

#endif // CONFIGDIALOG_H
