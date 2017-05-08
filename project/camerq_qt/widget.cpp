#include "widget.h"
#include<QCamera>
#include<QCameraViewfinder>
#include<QVBoxLayout>

#include<QPixmap>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->resize(600,400);
    camera=new QCamera();

    QCameraViewfinder*view_finder=new QCameraViewfinder();
    camera->setViewfinder(view_finder);
    QVBoxLayout*layout=new QVBoxLayout(this);

    layout->addWidget(view_finder);

    camera->start();
    camera->lockStatus(QCamera::LockWhiteBalance);


}

Widget::~Widget()
{

}


