#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QDebug>
int main(int argc, char *argv[])
{
//    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
//    QTextCodec::setCodecForLocale(codec);

   QTextCodec *_textCodec= QTextCodec::codecForLocale();//GBK
   qDebug()<<_textCodec->name();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
