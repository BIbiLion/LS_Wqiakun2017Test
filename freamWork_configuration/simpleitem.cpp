#include "simpleitem.h"
#include <QPainter>
#include <QGraphicsSceneDragDropEvent>
SimpleItem::SimpleItem(QGraphicsView * view):
QGraphicsItem(0)
{

    this->setFlag(ItemIsMovable,true);
     setFlags(  ItemIsMovable| ItemIsSelectable  | ItemIsFocusable| ItemSendsScenePositionChanges);
    m_view=view;
   // m_color=QColor( 255,255,255);
    setAcceptDrops(true);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);

  m_isEmpty=false;
}

  void SimpleItem::setSize(double width,double height)
  {
      m_height=height;
      m_width=width;
  }


QRectF SimpleItem::boundingRect() const
{
    qreal penWidth = 1;
//    return QRectF(-10 - penWidth / 2, -10 - penWidth / 2,
//                  20 + penWidth, 20 + penWidth);
    return QRectF(0,0,500,500);
}
void SimpleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget)
{/*
    painter->setPen(Qt::red);
    painter->drawRoundedRect(m_view->geometry().x(), m_view->geometry().y(),m_view->geometry().width() , m_view->geometry().height(), 5, 5);*/

   if(!m_isEmpty)
   {
    QRectF rectangle(10.0, 20.0, 200.0, 200.0);
    //painter->fillRect(rectangle,Qt::blue);
    painter->setPen(Qt::red);
    painter->drawRoundedRect(rectangle, 20.0, 15.0);
   }
   else
   {
   //利用双缓存得到以前的，并设置新的
     QImage _image(":/image/Lighthouse.jpg");
    _image= _image.scaledToHeight(100);
    _image= _image.scaledToWidth(100);
     painter->drawImage(0,0,_image);
   }
}

#include <QDebug>
 void SimpleItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
 {
   qDebug()<<"enter............";
   setToolTip("hello");
 }
  void SimpleItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
  {
    qDebug()<<"move ......";
  }

  void SimpleItem::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
  {
       event->setAccepted(true);
       update();

  }

  void SimpleItem::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
  {
     event->setAccepted(true);
     update();
  }

  void SimpleItem::dropEvent(QGraphicsSceneDragDropEvent *event)
  {
     // event->setAccepted(true);
     // update();

     m_isEmpty=true;
      //

  }
