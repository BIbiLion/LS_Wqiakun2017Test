#include "mainwindow.h"
#include <QApplication>

//#include "qwebkittest.h"
#include "qwebkittest_test.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    QWebKitTest mywk;
//    mywk.show();
    QWebKitTest_test WT_T;
    WT_T.show();

    return a.exec();
}
