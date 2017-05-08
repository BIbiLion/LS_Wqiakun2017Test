#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWidget>
#include <QToolButton>

#include "toolbutton.h"
#include "mygraphicsview.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

  //void dragEnterEvent(QDragEnterEvent *);


private:
    Ui::MainWindow *ui;

private:
    QGraphicsView * m_graphicsView;
    QGraphicsScene *m_graphicsScene;
   // QToolButton * m_toolButton;
     ToolButton * m_toolButton;

     MyGraphicsView * m_view;
};

#endif // MAINWINDOW_H
