#ifndef FTPMANAGER_H
#define FTPMANAGER_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QFile>
#include <QTcpSocket>
#include "qftp.h"

class FtpManager : public QObject
{
    Q_OBJECT
public:
    explicit FtpManager(QString _host, QString userName = "", QString passWd = "",qint16 _port = 21,QObject *parent = 0);
    ~FtpManager();

signals:
    void G_getProgressVal(int);

public slots:
    void S_commandFinish(int,bool);
    void S_upDateProgress(qint64,qint64);

    void S_abort();
    void S_dloadFile(QString _remoteFile,QString _localFile,bool isRese = false);
    void S_uloadFile(QString _localFile,QString _remoteFile,bool isRese = false);

//    void S_uLoadDir(QString _localDir,QString _remoteDir);
//    void S_dloadDir(QString _remoteDir,QString _localDir);

    //后期添加的方法
    void S_list();
    void S_listInfo(QUrlInfo);

    //
    void deleteFile(QString);

private:

    QString         m_userName;
    QString         m_passwd;

    QFtp            *myFtp;
    QFile           *m_File;
    bool            m_IsOpenFile;

};


#endif // FTPMANAGER_H
