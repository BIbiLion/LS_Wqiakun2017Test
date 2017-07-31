#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"

#include "writefile.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setupPlot();
private slots:
   void  resetPlot();
   void resetMoveXaxisPlot();
private:
    Ui::MainWindow *ui;
    int m_count;
    QVector<double > m_x;
    QVector<double > m_y;
    QWriteFile * m_writeFile;


};

#endif // MAINWINDOW_H
