#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <QWidget>
#include <QToolButton>
class ToolButton : public QToolButton
{
    Q_OBJECT
public:
    explicit ToolButton(QWidget *parent = 0);

//    void dragEnterEvent(QDragEnterEvent *);
//    void dragLeaveEvent(QDragLeaveEvent *);
//    void dragMoveEvent(QDragMoveEvent *);

//    void dropEvent(QDropEvent *);

    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);

signals:

public slots:
};

#endif // TOOLBUTTON_H
