
/*
1）通过静态hase()方法计算
    QByteArray byte_array;
    byte_array.append("password");
    QByteArray hash_byte_array = QCryptographicHash::hash(byte_array, QCryptographicHash::Md5);
    QString md5 = hash_byte_array.toHex();


（2）通过result()方法计算
    QByteArray byte_array;
    byte_array.append("password");
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(byte_array);  //添加数据到加密哈希值
    QByteArray result_byte_array = hash.result();  //返回最终的哈希值
    QString md5 = result_byte_array.toHex();
  */

#include <QCoreApplication>
#include <QDebug>
#include <QCryptographicHash>

#include <QFile>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
       QFile _file("E:/LSwuqiankun/LS_Wqiakun2017/LS_Wqiakun2017Test/Md5/WinMD5.exe");
       if(_file.exists())
       {

          if(!_file.open(QIODevice::ReadOnly))
          {
                qDebug()<<"canot open";
          }
       }
    //1）通过静态hase()方法计算
    QByteArray byte_array;
    byte_array.append(_file.readAll());//"password"
    QByteArray hash_byte_array = QCryptographicHash::hash(byte_array, QCryptographicHash::Md5);
    QString md5 = hash_byte_array.toHex();
     qDebug()<<md5;

    //QFile _file("E:\\LSwuqiankun\\LS_Wqiakun2017\\LS_Wqiakun2017Test\\Md5\\WinMD5.exe");// 944a1e869969dd8a4b64ca5e6ebc209a
     QFile _file2("E:/LSwuqiankun/LS_Wqiakun2017/LS_Wqiakun2017Test/Md5/三.rar");//
     if(_file2.exists())
     {

        if(!_file2.open(QIODevice::ReadOnly))
        {
              qDebug()<<"canot open";
        }
     }
    QCryptographicHash hash(QCryptographicHash::Md5);
    bool _isRight= hash.addData(&_file2);  //添加数据到加密哈希值
    QByteArray result_byte_array = hash.result();  //返回最终的哈希值
    QString md5File = result_byte_array.toHex();
    qDebug()<<_isRight<<md5File;

    _file.close();
    return a.exec();
}
