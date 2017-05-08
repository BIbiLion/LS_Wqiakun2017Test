#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
class QCamera;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    QCamera *camera;


};

#endif // WIDGET_H
