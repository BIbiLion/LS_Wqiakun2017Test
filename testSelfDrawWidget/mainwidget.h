#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = 0);


signals:

public slots:
protected:
    void paintEvent(QPaintEvent * event);
    void drawWarZoneTitle(QPainter *painter);
    void drawFunctionBtns(QPainter *painter);
    void  dataReceived(QPainter *painter);

    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

};

#endif // MAINWIDGET_H
