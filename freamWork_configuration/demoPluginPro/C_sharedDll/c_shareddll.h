#ifndef C_SHAREDDLL_H
#define C_SHAREDDLL_H

#include "c_shareddll_global.h"
#include <QWidget>
#include <QPushButton>
#include <QLabel>
class C_SHAREDDLLSHARED_EXPORT C_sharedDll
{

public:
    C_sharedDll();
   virtual QWidget * createWidget();
};

//很重要 切记
extern "C" C_SHAREDDLLSHARED_EXPORT QString getName();
extern "C" C_SHAREDDLLSHARED_EXPORT C_sharedDll* getTestDll(); //获取类TestDll的对象
#endif // C_SHAREDDLL_H
