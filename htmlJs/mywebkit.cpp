#include "MyWebKit.h"


MyWebKit::MyWebKit(QObject *parent):
    QObject(parent)
{
    ;
}

MyWebKit::~MyWebKit()
{
    ;
}
#include <QDebug>
#include "qwebkittest_test.h"
 void MyWebKit::onCall(QString strName,QString strPwd)
 {
   qDebug()<<strName;
  ((QWebKitTest_test*)this->parent())->setValueFromWeb(strName,strPwd);
 }

void MyWebKit::OnDataCallback(QString strBussid,QString strType,QString strIndex,QString strName,QString strValue)
{
    qDebug()<<strName<<strValue;
    //CCrEnvironmengDlg *pMain = (CCrEnvironmengDlg*)parent();
    //pMain->setValueFromWeb(strBussid,strType);
}
