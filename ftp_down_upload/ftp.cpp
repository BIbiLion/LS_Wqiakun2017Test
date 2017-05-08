#include "ftp.h"

Ftp::Ftp(QObject *parent)
    :QFtp(parent)
{
}
 void Ftp::initFtpClient(QString userName,QString password,QString  ipStr,int port)
 {
    this->connectToHost(ipStr,port);
    this->login(userName,password);

 }

 void Ftp::downFile(QString& remoteFile,QFile *localFile,qulonglong size)
 {
     //this->get(remoteFile,localFile,size);
 }

  void Ftp::putFile(QIODevice *dev,QString file)
  {
      this->put(dev,file);
  }
