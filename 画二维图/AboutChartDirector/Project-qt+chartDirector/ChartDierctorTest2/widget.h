#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include"cpiechart.h"
#include"chistogram.h"
#include"clinechart.h"
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_ptnsavedir_clicked();

    void on_ptnshowimg_clicked();


     QWidget * addLinearMeters();
    QWidget *  addPyramid();
    QWidget *  addPolar();
     QWidget * addScatter();
     QWidget * addContourColor();

    QWidget * addSwapXY();
   QWidget * addLineWithErrorSymbols();
   QWidget*   addSurface();
    QWidget*  addAngularMeter();
private:
    Ui::Widget *ui;
    Cpiechart *m_piechart;
    Chistogram *m_histogram;
    Clinechart *m_linechart;


    void setSavePath();
    //config *m_config;
};

#endif // WIDGET_H
