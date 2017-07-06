#include "testobject.h"
#include <QDebug>
#include <QThread>

/*
timerEvent：输出的threadId相同,也就是说他们在同一个线程中
QTimer 和他们输出的threadId相同
*/
#include <QTimer>
TestObject::TestObject(QObject *parent) :
    QObject(parent)
{
    qDebug()<<"identifierID1:"<<startTimer(5000);
    qDebug()<<"identifierID2:"<<startTimer(1000);
    qDebug()<<this->thread()->currentThreadId();

  QTimer::singleShot(0, this, SLOT(timeSlot()));
}
TestObject::~TestObject()
{

}

void TestObject::timerEvent(QTimerEvent *event)
{
  qDebug()<<"identifier:"<<event->timerId();
  qDebug()<<"threadId:"<<this->thread()->currentThreadId();
}

void TestObject::timeSlot()
{
 qDebug()<<"threadID timerOut:"<<this->thread()->currentThreadId();
}
