#include "toolbutton.h"

ToolButton::ToolButton(QWidget *parent) : QToolButton(parent)
{
 // this->setAcceptDrops(true);

}



void ToolButton::mousePressEvent(QMouseEvent *)
{
    setCursor(Qt::ClosedHandCursor);
}

#include <QDrag>
#include <QPainter>
#include <QMimeData>
#include <QMouseEvent>
#include <QBitmap>
void ToolButton::mouseMoveEvent(QMouseEvent *event)
{
    QDrag *drag = new QDrag(this);
    QMimeData *mime = new QMimeData;
    drag->setMimeData(mime);


    //设置跟随鼠标的图像
//    QPixmap pixmap(34, 34);
//    pixmap.fill(Qt::white);

//    QPainter painter(&pixmap);
//    painter.translate(15, 15);
//    painter.setRenderHint(QPainter::Antialiasing);

//    painter.setPen(Qt::NoPen);
//    painter.setBrush(Qt::darkGray);
//    painter.drawEllipse(-12, -12, 30, 30);
//    painter.setPen(QPen(Qt::black, 1));
//    painter.setBrush(QBrush(Qt::red));
//    painter.drawEllipse(-15, -15, 30, 30);
//    painter.end();
//    pixmap.setMask(pixmap.createHeuristicMask());
//    drag->setPixmap(pixmap);
//    drag->setHotSpot(QPoint(15, 20));

     QPixmap _pix(":/image/Lighthouse.jpg");
     _pix= _pix.scaled(50,50);
     drag->setPixmap(_pix);
     drag->setHotSpot(QPoint(15, 20));

    drag->exec();
    setCursor(Qt::OpenHandCursor);
}


 void ToolButton::mouseReleaseEvent(QMouseEvent *)
 {
       setCursor(Qt::OpenHandCursor);
 }
