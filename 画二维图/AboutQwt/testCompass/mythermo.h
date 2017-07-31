#ifndef MYTHERMO_H
#define MYTHERMO_H
#include <qwt_thermo.h>
#include <QWidget>
#include <Qevent>
#include <QPaintEvent>
#include <QPainter>
class myThermo : public QwtThermo
{
public:
    myThermo();
     myThermo(QWidget*widget);
protected:
    void paintEvent( QPaintEvent * );
    virtual void drawLiquid( QPainter *, const QRect & ) const;

};

#endif // MYTHERMO_H
