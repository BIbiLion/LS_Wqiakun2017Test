#include <QCoreApplication>

#include "md5.h"

#include <QDebug>
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if(argc !=2)
    {
        qDebug()<<"need filePath !!!";
        return 0;

    }

    MD5_CTX _md5;

    char md5[50];
    memset(md5,0,50);
    QString _filePath(QString::fromLocal8Bit( argv[1]));  //argv[1]  //73f48840b60ab6da68b03acd322445ee  原始的MD5值 "./winmd5free.rar"

    bool _flag= _md5.GetFileMd5(md5,_filePath.toLocal8Bit().constData());//计算出来的MD5值

    QString _str(md5);
   // qDebug()<<_flag<<md5<<_str;
 qDebug()<<"md5: "<<_str;
    return a.exec();
}
