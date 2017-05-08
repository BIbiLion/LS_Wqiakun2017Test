#include "mainwidget.h"
#include <QPainter>
MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
{
}

void MainWidget::paintEvent(QPaintEvent * event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform);
    drawWarZoneTitle(&painter);
    drawFunctionBtns(&painter);
}

#include <QColor>
void MainWidget::drawWarZoneTitle(QPainter *painter)
{
    painter->save();
    painter->setPen(Qt::SolidLine);
    painter->setBrush(QBrush(Qt::green, Qt::Dense3Pattern));
    static const QPointF points[8] = {
        QPointF(10.0, 80.0),
        QPointF(200.0, 80.0),
        QPointF(230.0, 110.0),
        QPointF(220.0, 120.0),
        QPointF(180.0, 120.0),
        QPointF(170.0, 110.0),
        QPointF(30.0, 110.0),
        QPointF(10.0, 130.0)
    };
    painter->drawPolygon(points,8);
    // painter->drawRoundRect(rect(), 0, 0);

    painter->restore();
    painter->drawText(QPointF(30,100),"南部展区A展厅第3展示台");




    //painter->drawPixmap(10,50,QPixmap(":/image/image/warzone_click.png"));
    //painter->drawPixmap(0,0,QPixmap(":/image/image/frame.png"));//颜色深

    QPixmap _pixmap(600,800);
    _pixmap.fill(QColor(255,255,0));
    QPainter _comPainter(&_pixmap);

    _comPainter.drawLine( QPointF(0.0, 0.0),
                        QPointF(100.0, 130.0));

    _pixmap.save("./avePixmap.png");
    // painter->drawLine(QPointF(100,90),QPointF(200,90));
}

#include <QStyleOptionButton>
#include <QApplication>
void MainWidget::drawFunctionBtns(QPainter *painter)
{
    painter->save();
    painter->setPen(Qt::SolidLine);
    painter->setBrush(QBrush(Qt::green, Qt::Dense3Pattern));
    static const QPointF points[8] = {
        QPointF(205.0, 80.0),
        QPointF(500.0, 80.0),
        QPointF(450.0, 120.0),
        QPointF(400.0, 120.0),
        QPointF(380.0, 110.0),
        QPointF(350.0, 110.0),
        QPointF(300.0, 120.0),
        QPointF(245.0, 120.0)
    };
    painter->drawPolygon(points,8);
    // painter->drawRoundRect(rect(), 0, 0);
    painter->restore();

    QStyleOptionButton* button1 = new QStyleOptionButton();
    button1->state |= QStyle::State_Enabled;
    QRect rect(180,60,220,85);
    QApplication::style()->drawItemPixmap(painter,rect,Qt::AlignLeft|Qt::AlignCenter,QPixmap(":/image/close.png"));

    QApplication::style()->drawControl(QStyle::CE_PushButton,button1 , painter);

}

void  MainWidget::dataReceived(QPainter *painter)
{

}


void  MainWidget::mousePressEvent(QMouseEvent *)
{

}

void  MainWidget::mouseReleaseEvent(QMouseEvent *)
{

}

