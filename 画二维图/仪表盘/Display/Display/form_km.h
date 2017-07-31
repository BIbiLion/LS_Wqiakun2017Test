#ifndef FORM_KM_H
#define FORM_KM_H

#include <QWidget>
#include <QPainter>
namespace Ui {
class Form_KM;
}

class Form_KM : public QWidget
{
    Q_OBJECT

public:
    explicit Form_KM(QWidget *parent = 0);
    ~Form_KM();


    void paintEvent(QPaintEvent *);

    void change_Speed(float temp);
private:
    Ui::Form_KM *ui;
    int m_pointX;
    int m_pointY;
    float m_degreespeed;
    float m_degreeRotate;

};

#endif // FORM_KM_H
