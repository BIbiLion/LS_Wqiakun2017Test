#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   // MainWindow w;
    //w.show();
    QString _str("北京");
    qDebug()<<_str;

    return a.exec();
}
