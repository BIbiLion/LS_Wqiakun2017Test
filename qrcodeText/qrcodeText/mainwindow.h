#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
//    void setString(QString str);
//    bool saveImage(QString fileName, int size);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

protected:
//    void paintEvent(QPaintEvent *);
//    QSize sizeHint() const;
//    QSize minimumSizeHint() const;
private:
    Ui::MainWindow *ui;

//    void draw(QPainter &painter, int width, int height);
//    QString string;
//    QRcode *qr;

};

#endif // MAINWINDOW_H
