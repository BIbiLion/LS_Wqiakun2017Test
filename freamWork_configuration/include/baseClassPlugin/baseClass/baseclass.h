#ifndef BASECLASS_H
#define BASECLASS_H

//#include <QWidget>
/*
纯虚基类 所有类的基类 定义借口
*/
class baseClass
{
public:
    explicit baseClass(){};
    virtual QWidget * createWidget() =0;
    virtual QString  hhh() =0;
};

#endif // BASECLASS_H
