#include "c_shareddll.h"


C_sharedDll::C_sharedDll()

{
}

QWidget * C_sharedDll::createWidget()
{
    QLabel * _lable = new QLabel();

    _lable->setText("hello createDll widget");
    return _lable;
}

QString C_sharedDll::hhh()
{
   return QString("pure iniher");
}

//导出全局函数
QString  getName()
{
    return QString("wuqiankun");
}

//导出类
C_sharedDll* getTestDll()
{
    return new C_sharedDll();
}
