#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>
#include <QTimer>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


   QTimer * _timer= new QTimer(this);
   _timer->setInterval(2000);
   connect(_timer,SIGNAL(timeout()),this,SLOT(showpixmap()));
   _timer->start();
   m_beginSize=QSize(0,0);
}

MainWindow::~MainWindow()
{
    delete ui;
}
#include <QDebug>
#include <QMatrix>
#include <QDateTime>
void MainWindow::showpixmap()
{
     QPixmap _pixMap("./card.png");

     QPixmap _tmpPixmap=_pixMap.copy(m_beginSize.width(),m_beginSize.height(),80,105);//m_beginSize.width(),m_beginSize.height()

     QMatrix m;//创建一个QWMatrix类的对象

     //_tmpPixmap = _tmpPixmap.scaled(200,200);
     QString _str=QDateTime::currentDateTime().toString("YYYY-MM-dd-hh-mm-ss.png");
     _tmpPixmap.save(_str);


     ui->label->setPixmap(_tmpPixmap);
qDebug()<<_pixMap.width()<<_pixMap.height() <<m_beginSize.width()<<m_beginSize.height();
     if((m_beginSize.width()+80)>= _pixMap.width())
     {
        m_beginSize+=QSize(0,105);
        m_beginSize.setWidth(0);
     }
     else
     {
       m_beginSize+=QSize(80,0);
     }

     if(m_beginSize.height()>105*5)
     {
      m_beginSize=QSize(0,0);
     }
}
