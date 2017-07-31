#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mythermo.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setCompass();
    void setDial();
private slots:
    void changgeValue();
private:
    Ui::MainWindow *ui;
     myThermo * mm;
     int m_value;
};

#endif // MAINWINDOW_H
