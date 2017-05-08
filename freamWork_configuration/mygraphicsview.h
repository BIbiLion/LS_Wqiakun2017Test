#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>

class MyGraphicsView : public QGraphicsView
{
public:
    MyGraphicsView(QGraphicsScene *scene);
   void dragEnterEvent(QDragEnterEvent * event);
//   void dragMoveEvent(QDragMoveEvent *event);
//   void dropEvent(QDropEvent *event);
};

#endif // MYGRAPHICSVIEW_H
