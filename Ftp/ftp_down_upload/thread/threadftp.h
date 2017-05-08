#ifndef THREADFTP_H
#define THREADFTP_H

#include <QObject>
#include "qftp.h"
#include <QFile>
#include <QStringList>
#include "qurlinfo.h"\

#include <QSemaphore>
/*
  FTP的工作线程类
*/
class ObjectFtp : public QObject
{
    Q_OBJECT
public:
    enum
    {
        UpLoad,
        DownLoad,
    };
    explicit ObjectFtp(QObject *parent = 0);
    ~ObjectFtp();

    void initFtpClient(QString userName,QString password,QString  ipStr,int port=21);


    void downDir(QString downDir);

    void connectToHost(QString ipstr,int port);
    void login(QString userName,QString password);


    bool isExist(QString _dir);
    bool mkDir(QString dir);
    void listDir(QString dir=QString());
protected:
    void putFile(QString filePath, QString file);
    void putFile(QIODevice *dev,QString file);

    void downFile(QString remoteFileName);
    void downFile(QString& remoteFile,QFile *localFile,qulonglong size=0);
signals:
    void progressValue(qint64 value,qint64 total);
    void listUrlInfoList( QList<QUrlInfo>);
    void downLoadFinshed();
    void uploadFinshed();
    void mkdirSucceed();
public slots:

    void commandStartedSlot(int);
    void commandFinshed(int,bool);

    void listInfoSlot(QUrlInfo);
    void uploadDataTransferProgressSlot(qint64,qint64);


    void putFileList(QStringList fileList);
private:
    QFtp * m_ftp;
    QList<QUrlInfo> m_urlInfoList;
    QList<QUrlInfo> m_DirurlInfoList;

    QFile * m_file;
    int m_StartCommandIndex;

    QStringList m_uploadFileList;
    bool m_flag;
    bool m_isUploadOrDown;//upload true;  dwon false
    QList<QUrlInfo>  m_downFileUrlInfoList;//下载文件列表
    QString m_downDir;

};

#endif // THREADFTP_H
