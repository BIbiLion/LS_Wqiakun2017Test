/*
函数名： wuqiankun 实现Ftp文件的上传和下载功能，可以使用续传的
借用了道友的ftp源码
*/

#include "mainwindow.h"
#include <QApplication>

#include "Widget/widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();


    widget ww;
    ww.show();
    return a.exec();
}
