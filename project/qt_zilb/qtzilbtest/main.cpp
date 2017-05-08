#include <QApplication>
#include "mainwindow.h"
#include "qtextcodec.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //QTextCodec *pcoder=QTextCodec::codecForName("gb18030");
    //QTextCodec::setCodecForCStrings(pcoder);
   // QTextCodec::setCodecForLocale(pcoder);
    //QTextCodec::setCodecForTr(pcoder);

    MainWindow w;
    w.show();

    return a.exec();
}
