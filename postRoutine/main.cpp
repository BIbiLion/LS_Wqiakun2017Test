/*
 *此函数主要是为了
 * 添加一个将被QApplication析构函数中调用的全局日常事务。这个函数通常用来添加在整个程序范围的功能的清理的日常事务。
   主要针对全局变量 静态变量

   对于模块和库，使用引用计数的初始化管理器或者Qt的父子对象删除机制也许更好。
*/

#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
static int *global_ptr = 0;

static void cleanup_ptr()
{
    qDebug()<<"enter the Function..........";
    delete [] global_ptr;
    global_ptr = 0;
}

void init_ptr()
{
    global_ptr = new int[100];      // 分配数据
    qAddPostRoutine( cleanup_ptr ); // 稍后删除
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    init_ptr();
    MainWindow w;
    w.show();

    return a.exec();
}
