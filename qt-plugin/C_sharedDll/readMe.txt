//添加 全局函数的导出

//c_shareddll.h

#ifndef C_SHAREDDLL_H
#define C_SHAREDDLL_H

#include "c_shareddll_global.h"
#include <QWidget>
#include <QPushButton>
class C_SHAREDDLLSHARED_EXPORT C_sharedDll
{

public:
C_sharedDll();
QWidget * createWidget();
};

//很重要 切记
extern "C" C_SHAREDDLLSHARED_EXPORT QString getName();
#endif // C_SHAREDDLL_H

//c_shareddll.cpp
#include "c_shareddll.h"


C_sharedDll::C_sharedDll()
{
}

 QWidget * C_sharedDll::createWidget()
 {
 QWidget * _widget = new QWidget();
   _widget->show();
 }

  QString  getName()
  {
  return QString("wuqiankun");
  }


//采用显示链接，调用C++类中的类对象、成员函数

