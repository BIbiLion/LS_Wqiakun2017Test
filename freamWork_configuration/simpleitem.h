#ifndef SIMPLEITEM_H
#define SIMPLEITEM_H

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QColor>
class SimpleItem : public QGraphicsItem
{
public:
    SimpleItem(QGraphicsView * view);
    void setSize(double width,double height);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);


    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);

    void  hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

private:
    double m_height;
    double m_width;

    QColor m_color;
    QGraphicsView * m_view;
    bool m_isEmpty;
};

#endif // SIMPLEITEM_H
