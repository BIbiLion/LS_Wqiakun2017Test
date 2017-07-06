#ifndef MYWEBKIT_H
#define MYWEBKIT_H
#include <QObject>
//#include "qwebkittest_test.h"
//class QWebKitTest_test;

class MyWebKit : public QObject
{
    Q_OBJECT

public:
    MyWebKit(QObject *parent=0);
    ~MyWebKit();

public slots:

    void onCall(QString strName,QString strPwd);
   void OnDataCallback(QString strBussid,QString strType,QString strIndex,QString strName,QString strValue);
private:

};
#endif // MYWEBKIT_H
