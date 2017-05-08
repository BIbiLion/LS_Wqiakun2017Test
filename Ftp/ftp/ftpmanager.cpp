#include "ftpmanager.h"
#include <QDebug>


class FtpManager;

FtpManager::FtpManager(QString _host, QString userName, QString passWd, qint16 _port, QObject *parent):
    QObject(parent),
    m_userName(userName),
    m_passwd(passWd),
    m_File(0),
    m_IsOpenFile(false)
{
    //构建ftp对象
    myFtp = new QFtp(this);
    //连接ftp服务器
    myFtp->connectToHost(_host,_port);
    //进度条显示
    connect(myFtp,SIGNAL(dataTransferProgress(qint64,qint64)),
            SLOT(S_upDateProgress(qint64,qint64)));
    //状态显示
    connect(myFtp,SIGNAL(commandFinished(int,bool)),
            SLOT(S_commandFinish(int,bool)));

    //文件列表
    connect(myFtp,SIGNAL(listInfo(QUrlInfo)),SLOT(S_listInfo(QUrlInfo)));
}

FtpManager::~FtpManager()
{
    delete myFtp;
}

//停止Ftp动作
void FtpManager::S_abort()
{
    myFtp->abort();
}

void FtpManager::S_list()
{
    myFtp->list();
}


void FtpManager::deleteFile(QString remoteFile)
{
    myFtp->remove(remoteFile);
}

//文件列表
void FtpManager::S_listInfo(QUrlInfo _urInfo)
{
    qDebug() <<_urInfo.name()/*.toLatin1()*/<<" "<<_urInfo.size()<<" " <<_urInfo.owner();
}

//下载文件（当isRese==true为续传下载）
void FtpManager::S_dloadFile(QString _remoteFile,QString _localFile,bool isRese)
{
    m_File = new QFile(_localFile);

    if(!isRese)
    {
        qDebug() << tr("文件%1的普通下载... ...").arg(_remoteFile);
        if(m_File->open(QIODevice::WriteOnly))
        {
            m_IsOpenFile = true;
            myFtp->get(_remoteFile,m_File);
        }
        else
        {
            delete m_File;
            m_File = NULL;
            qDebug() << tr("本地文件%1打开失败！").arg(_localFile);
        }
    }
    else
    {
        qDebug() << tr("文件%1的续传下载... ...").arg(_remoteFile);
        if(m_File->open(QIODevice::Append))
        {
            m_IsOpenFile = true;
            myFtp->rawCommand(tr("REST %1").arg(m_File->size()));
            myFtp->m_isConLoad = true;          //设置当前现在为续传
            myFtp->get(_remoteFile,m_File);
        }
        else
        {
            delete m_File;
            m_File = NULL;
            qDebug() << tr("本地文件%1打开失败").arg(_localFile);
        }
    }
}

//上传文件（当isRese==true为续传上传）
void FtpManager::S_uloadFile(QString _localFile,QString _remoteFile,bool isRese)
{
    m_File = new QFile(_localFile);

    if(m_File->open(QIODevice::ReadOnly))
    {
        m_IsOpenFile = true;
        if(!isRese)
        {
            qDebug() << tr("文件%1的普通上传... ...").arg(_localFile);
            myFtp->put(m_File,_remoteFile);                                     //上传
        }
        else
        {
            qDebug() << tr("文件%1的续传... ...").arg(_localFile);
            //在QFtp中定义续传方法
            qDebug()<<_remoteFile<<QString::fromLatin1(_remoteFile.toLocal8Bit());
            myFtp->conPut(m_File,_remoteFile);                                  //续传
        }
    }
    else
    {
        delete m_File;
        m_File = NULL;
        qDebug() << tr("本地文件%1打开失败！").arg(_localFile);
    }
}



//更新进度条
void FtpManager::S_upDateProgress(qint64 _used, qint64 _total)
{
    int tmpVal = _used / (double)_total * 100;
    emit G_getProgressVal(tmpVal);
}

//ftp服务提示信息
void FtpManager::S_commandFinish(int tmp, bool en)
{
    Q_UNUSED(tmp);

    if(myFtp->currentCommand() == QFtp::ConnectToHost){

        if(en)
            qDebug() << (tr("连接服务器出现错误：%1").arg(myFtp->errorString()));

        else
        {
            qDebug() << (tr("连接到服务器成功"));
            myFtp->login(m_userName,m_passwd);                           //登陆服务器
        }
    }

    if (myFtp->currentCommand() == QFtp::Login){

        if(en)
            qDebug() << (tr("登录出现错误：%1").arg(myFtp->errorString()));

        else
            qDebug() << (tr("登录服务器成功"));

    }

    if (myFtp->currentCommand() == QFtp::Get)
    {

        if(en)
        {
            qDebug() << (tr("下载出现错误：%1").arg(myFtp->errorString()));
        }

        else
        {
            qDebug() << (tr("已经完成下载"));
            m_File->flush();
        }

        m_File->close();
        m_IsOpenFile = false;
        delete m_File;
        m_File = NULL;
    }
    else if(myFtp->currentCommand() == QFtp::Put)
    {

        if(en)
        {
            qDebug() << (tr("上传出现错误：%1").arg(myFtp->errorString()));
        }
        else
        {
            qDebug() << (tr("已经完成文件上传"));
        }

        m_File->close();
        m_IsOpenFile = false;
        delete m_File;
        m_File = NULL;
    }
    else if (myFtp->currentCommand() == QFtp::Close)
    {

        qDebug() << (tr("已经关闭连接"));

        if(m_IsOpenFile)
        {
            m_File->close();
            delete m_File;
            m_File = NULL;
        }
    }
    else if(myFtp->currentCommand() == QFtp::Remove)
    {
      qDebug()<<"removete File";
    }
}
