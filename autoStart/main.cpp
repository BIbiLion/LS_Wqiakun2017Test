#include <QCoreApplication>
#include <QSettings>
//修改注册表  如果注册表不存在，再删除也不会发生错误。
#define REG_RUN "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"

void setAutoStart(bool is_auto_start)
{

    QString application_name = QCoreApplication::applicationName();
    QSettings *settings = new QSettings(REG_RUN, QSettings::NativeFormat);
    if(is_auto_start)
    {
        QString application_path = QCoreApplication::applicationFilePath();
        settings->setValue(application_name, application_path.replace("/", "\\"));
    }
    else
    {
        settings->remove(application_name);
    }
    delete settings;
    settings=NULL;
}
#include <QDebug>

#include "savefile.h"
#include <QTextStream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    /*
     //test1  应用开机自启动
    setAutoStart(true);
    QString _str("0123456789");

    qDebug()<<_str.left(5)<<_str.mid(5);
    QByteArray _by;
    _by.resize(2);
    _by[0]=0xeb;
    _by[1]=0x90;
    */

    // return a.exec();
    QTextStream standardOutput(stdout);
    standardOutput<<"standardOutput";


    int i=0;
    while(i<10)
    {
        if(++i==9)//(++i)==9
        {
            standardOutput<<"test:"<<i<<"\n";
        }
        standardOutput<<i<<"\n";
    }
    qDebug()<<"+++";
    int j=10;
    while(j<20)
    {
        if(j++==15)//j==15 j++
        {
            standardOutput<<"test:"<<j<<"\n";
        }
        standardOutput<<j<<"\n";
    }

    //getchar();
    return 0;
}
