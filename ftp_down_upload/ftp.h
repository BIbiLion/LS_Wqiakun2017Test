#ifndef FTP_H
#define FTP_H

#include "qftp.h"
#include <QFile>
#include <QIODevice>
class Ftp : public QFtp
{
public:
    Ftp(QObject *parent = 0);
     void initFtpClient(QString userName,QString password,QString  ipStr,int port=21);
     void downFile(QString& remoteFile,QFile *localFile,qulonglong size=0);
     void putFile(QIODevice *dev,QString file);
private:

};

#endif // FTP_H
