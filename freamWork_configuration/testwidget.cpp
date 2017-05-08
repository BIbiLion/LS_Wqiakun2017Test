#include "testwidget.h"
#include <QPixmap>
#include <QPainter>
#include <QBitmap>
TestWidget::TestWidget(QWidget *parent) :
    QWidget(parent)
{
}


 void TestWidget::paintEvent(QPaintEvent *)
 {

//     QPixmap pixmap(34, 34);
//     pixmap.fill(Qt::white);

//     QPainter painter(&pixmap);
//     painter.translate(15, 15);
//     painter.setRenderHint(QPainter::Antialiasing);



     QPainter painter(this);
     painter.save();
     painter.translate(100,100);
     painter.setPen(Qt::NoPen);
     painter.setBrush(Qt::darkGray);
     painter.drawEllipse(-12, -12, 30, 30);

      painter.translate(200,200);
     painter.setPen(QPen(Qt::black, 1));
     painter.setBrush(QBrush(Qt::red));
     painter.drawEllipse(-15, -15, 30, 30);

    // pixmap.setMask(pixmap.createHeuristicMask());

    // painter.drawPixmap(50,50,50,50,pixmap);

    painter.restore();

 }
