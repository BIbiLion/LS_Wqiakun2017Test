#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void merge();
    void save();
private:
    Ui::MainWindow *ui;
    QImage m_pixmap_one;
    QImage m_pixmap_two;
    QImage *m_image;
};

#endif // MAINWINDOW_H
