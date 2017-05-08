#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qftp.h"

#include "ftp.h"
#include <QFile>

#include <QDir>
#include <QFileInfo>
#include "qurlinfo.h"

struct File
{
    QString fileDir;
    QString fileName;
};

#include <QUdpSocket>
namespace Ui {
class MainWindow;
}

#include <QSemaphore>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();
private slots:
    void listInfoSlot(QUrlInfo);
    void downFile(QString remoteFileName=QString());
    void dataTransferProgressSlot(qint64,qint64);

    void uploadFile(QString localFileName=QString());
    void uploadDataTransferProgressSlot(qint64,qint64);

    void finshedSlot(bool );
    QStringList getfileList(QDir *_dir=NULL);

    void commandStartedSlot(int);
    void commandFinshed(int,bool);

    void uploadServerlistInfoSlot(QUrlInfo);
    void mkRemoteDir(QString remoteDir);
    void stateChangeSlot(int);
    void uploadCommandStart(int);
    void uploadCommandFinshed(int ,bool );


    void sendNewCommand();

    void processPendingDatagrams();
protected:


private:

    Ui::MainWindow *ui;
    //QFtp * m_ftpDown;
    Ftp * m_ftp;
    Ftp* m_uploadFtp;


    QFile * m_file;
    QFile *m_uplodFile;
    QStringList m_remoteDirList;

    QUdpSocket *m_udpSocket;//udpSocket结束 服务器发来的下载文件路径
    QString m_remotePath;//远程服务器发送的文件夹路径

    QList<QUrlInfo> m_urlInfoList;
    QStringList m_fileList;//本地 需要发送的文件列表

    QString m_saveLocalPath;//本地存储路径

   int m_opIndex;
};

#endif // MAINWINDOW_H
