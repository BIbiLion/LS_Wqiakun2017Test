#include "mygraphicsview.h"


#include <QDebug>
MyGraphicsView::MyGraphicsView(QGraphicsScene *scene) : QGraphicsView(scene)
{

    this->setAcceptDrops(true);
}
void MyGraphicsView::dragEnterEvent(QDragEnterEvent * event)
{
    qDebug()<<"myView dragEnterEvent";
    QGraphicsView::dragEnterEvent(event);

}


//void MyGraphicsView::dragMoveEvent(QDragMoveEvent *event)
//{
//   qDebug()<<"myView dragMoveEvent";
//}

//void MyGraphicsView::dropEvent(QDropEvent *event)
//{
//   qDebug()<<"myView dropEvent";
//}
