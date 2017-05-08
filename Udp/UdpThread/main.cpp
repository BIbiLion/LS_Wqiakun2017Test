#include <QtGui/QApplication>
#include <QThread>
#include "udpobject.h"
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    QThread * _thread= new QThread;
    UdpObject * _udpObject=new UdpObject;
    _udpObject->moveToThread(_thread);
    QObject::connect(_thread,SIGNAL(started()),_udpObject,SLOT(startThread()));
    QObject::connect(_udpObject,SIGNAL(end()),_thread,SLOT(quit()));
    _thread->start();

    //no delete in this demo
    return a.exec();
}
