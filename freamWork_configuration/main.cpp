#include "mainwindow.h"
#include <QApplication>

#include "testwidget.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //TestWidget tw;
   // tw.show();

    return a.exec();
}
