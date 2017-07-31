#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}
#include <QPolygonF>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void paintEvent(QPaintEvent *);
private:
    Ui::MainWindow *ui;
    QPolygonF m_polygon;
     QPolygonF m_polygonLine;
};

#endif // MAINWINDOW_H
