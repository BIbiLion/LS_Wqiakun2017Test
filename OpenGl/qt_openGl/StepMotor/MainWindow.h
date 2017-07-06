#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "GLWidget.h"
#include <QMainWindow>
#include <QGridLayout>
#include <QWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QGridLayout *glayout;
    GLWidget glWidget;
    QWidget *widget;

public slots:
    void slotDisplayLed6to0(uint portData);
    void slotDisplayLed7(uint data);
    void slotFullScreen();
    void slotQuitFullScreen();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
