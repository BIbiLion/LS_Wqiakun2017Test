#include <QCoreApplication>
#include "QFile"
#include <QFileInfo>
#include <QDir>
#include <QDebug>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    /*
    QFile _file("F:/FtpPath/2017_01_16/log_0.dat");
    for(int i=0;i<30;++i)
    {
        _file.copy(QString("F:/FtpPath/2017_01_16/log_"+QString("%1").arg(i)+".dat"));
    }

    QFile _file2("F:/FtpPath/2017_01_16/mt_0.dat");
    for(int i=0;i<30;++i)
    {
        _file2.copy(QString("F:/FtpPath/2017_01_16/mt_"+QString("%1").arg(i)+".dat"));
    }

    QFile _file3("F:/FtpPath/2017_01_16/result_0.dat");
    for(int i=0;i<30;++i)
    {
        _file3.copy(QString("F:/FtpPath/2017_01_16/result_"+QString("%1").arg(i)+".dat"));
    }
    */

    QFileInfo _file3("F:/test/地面配电初始化2017-02-0417-54-10/log_15.dat");
    qDebug()<<_file3.absoluteDir();


    return a.exec();
}

