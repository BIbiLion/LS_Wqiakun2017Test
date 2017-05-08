#include "threadftp.h"
#include <QDebug>
#include <QApplication>
#include <QProcess>

#include <windows.h>
QSemaphore g_sem(1);
//bool m_flag=false;
//extern bool m_flag=true;
#include <QDir>

ObjectFtp::ObjectFtp(QObject *parent) :
    QObject(parent),m_flag(true)
{
    m_ftp= new QFtp(this);

}
ObjectFtp::~ObjectFtp()
{

    if(m_file !=NULL)
    {
        if(m_file->isOpen())
        {
            m_file->close();
        }
        m_file->deleteLater();
        m_file=NULL;
    }
    m_ftp->close();
    m_ftp->deleteLater();
    m_ftp=NULL;
}

void ObjectFtp::initFtpClient(QString userName,QString password,QString  ipStr,int port)
{
    m_ftp->connectToHost(ipStr,port);
    m_ftp->login(userName,password);
    //    connectToHost(ipStr,port);
    //    login(userName,password);
    connect(m_ftp,SIGNAL(listInfo(QUrlInfo)),this,SLOT(listInfoSlot(QUrlInfo )),Qt::DirectConnection);
    connect(m_ftp,SIGNAL(commandStarted(int)),this,SLOT(commandStartedSlot(int)),Qt::DirectConnection);
    connect(m_ftp,SIGNAL(commandFinished(int,bool)),this,SLOT(commandFinshed(int ,bool )),Qt::DirectConnection);
    connect(m_ftp,SIGNAL(dataTransferProgress(qint64,qint64)),this,SLOT( uploadDataTransferProgressSlot(qint64,qint64)));

    // connect(m_ftp,SIGNAL(stateChanged(int))
}


void ObjectFtp::connectToHost(QString ipstr,int port)
{
    int _timeout=0;
    while(m_flag== false || _timeout<10)
    {
        qApp->processEvents();
        // m_flag=true;
        Sleep(100);
        _timeout++;
    }

    m_ftp->connectToHost(ipstr,port);
    m_flag=true;
}

void ObjectFtp::login(QString userName,QString password)
{
    int _timeout=0;
    while(m_flag== false || _timeout<10)
    {
        qApp->processEvents();
        // m_flag=true;
        Sleep(100);
        _timeout++;
    }


    m_ftp->login(userName,password);
    m_flag=true;
}

void ObjectFtp::downDir(QString downDir)
{
    m_downDir=downDir;
    m_isUploadOrDown=false;
    listDir(downDir);
}


void ObjectFtp::downFile(QString remoteFileName)
{

   QString  localFileName =QApplication::applicationDirPath()+"/"+m_downDir+"/"+remoteFileName;

    if(QFile::exists(localFileName))
    {
        QFile::remove(localFileName);
    }
//当前目录是否存在

   QString _dirStr=QApplication::applicationDirPath()+"/"+m_downDir;
qDebug()<<".........."<<_dirStr;
    QDir _dir(_dirStr);
    if( !_dir.exists() )
    {
        _dir.mkpath(_dirStr);
    }

    qDebug()<<localFileName.toLatin1()<<localFileName.toLocal8Bit()<<localFileName.toUtf8();
    m_file= new QFile( localFileName);//modify

    if (!m_file->open(QIODevice::ReadWrite ))
    {
         qDebug()<<"not....";
        return;
    }
     QString _remoteDirName=(m_downDir+"\\"+ remoteFileName);
    downFile(_remoteDirName,m_file);
}

void ObjectFtp::downFile(QString& remoteFile,QFile *localFile,qulonglong size)
{
    m_ftp->get(remoteFile,localFile,size);
}


/*
函数名：传递文件列表
*/
void ObjectFtp::putFileList(QStringList fileList)
{
    m_isUploadOrDown=true;
    m_uploadFileList=fileList;
    QString _remotePath;
    QString _remoteDir;
    QString _remotefileName;

    for(int i=0; i<fileList.count();++i)
    {
        QString _headStr(QApplication::applicationDirPath()+"/" );
        int _headIndex=  fileList[i].indexOf(_headStr);
        _remotePath=fileList[i].mid(_headIndex+_headStr.size());//目前只有一层 比如 a\\b.txt

        int _index=_remotePath.lastIndexOf("/");
        if(_index != -1)
        {

            _remoteDir= _remotePath.left(_index);

            _remotefileName=_remotePath.mid(_index+1);
            QStringList _dirList=_remoteDir.split("/");
            for(int k=0;k<_dirList.count();++k)
            {

                listDir();//1层     //分层级创建

                int _timeout=0;
                while(m_flag== false || _timeout<10)
                {
                    qApp->processEvents();
                    // m_flag=true;
                    Sleep(100);
                    _timeout++;
                }


                bool _isMK=false;
                qDebug()<<"hhhhhhhhh"<<m_DirurlInfoList.count();
                for(int i=0;i<m_DirurlInfoList.size();++i)
                {
                    qDebug()<<"name"<<m_DirurlInfoList[i].name()<<_remoteDir;
                    if(m_DirurlInfoList[i].name()==_remoteDir)
                    {
                        qDebug()<<"this is equal\n";
                        _isMK=true;
                        break;
                    }
                }

                if(_isMK==false)
                {
                    mkDir(_remoteDir);
                }
            }
        }

        //putFile  uploadFile
        putFile(fileList[i],_remotePath);

    }


}






void ObjectFtp::putFile(QString filePath, QString file)
{
    m_file= new QFile(filePath);
    if( !m_file->open(QIODevice::ReadWrite))
    {
        qDebug()<<"not";
    }
    putFile(m_file,file);

}

void ObjectFtp::putFile(QIODevice *dev,QString file)
{

    m_ftp->put(dev,file);
}

bool ObjectFtp::isExist(QString _dir)
{



    return true;
}


bool ObjectFtp::mkDir(QString dir)
{
    int _timeout=0;
    while(m_flag== false || _timeout<10)
    {
        qApp->processEvents();
        // m_flag=true;
        Sleep(100);
        _timeout++;
    }
    qDebug()<<"enter mkDir";
    m_ftp->mkdir(dir);
    m_flag=false;
}

void  ObjectFtp::listDir(QString dir)
{
    m_DirurlInfoList.clear();
    int _timeout=0;
    while(m_flag== false || _timeout<10)
    {
        qApp->processEvents();
        // m_flag=true;
        Sleep(100);
        _timeout++;
    }


    m_ftp->list(dir);
    m_flag=false;
}


void ObjectFtp::commandStartedSlot(int)
{
    qDebug()<<"start:"<<m_ftp->currentCommand();
    m_flag=false;
    switch (m_ftp->currentCommand())
    {
    case QFtp::Login:
        break;

    case QFtp::List:
        break;


    case QFtp::Cd:

        break;
    case QFtp::Get:

        break;

    case QFtp::Put:
        break;

    case QFtp::Mkdir:

        break;
    default:
        break;
    }
}

void ObjectFtp::commandFinshed(int type,bool error)
{
    qDebug()<<"end:"<<m_ftp->currentCommand();
    switch (m_ftp->currentCommand())
    {
    case QFtp::ConnectToHost:
        if(!error)
        {
            m_flag=true;
        }
        break;

    case QFtp::Login:

        if(!error)
        {
            m_flag=true;
        }
        break;

    case QFtp::List:
        if(!error)
        {
            emit listUrlInfoList(m_urlInfoList);
            m_flag=true;
        }

        if(!m_isUploadOrDown)//
        {
          m_downFileUrlInfoList=m_urlInfoList;

          if(!m_downFileUrlInfoList.isEmpty())
          {
              downFile(m_downFileUrlInfoList[0].name());
          }
        }

        break;
    case QFtp::Cd:

        break;
    case QFtp::Get:

        if(m_file !=NULL && m_file->isOpen())
        {
            m_file->close();
            m_file->deleteLater();;
            m_file=NULL;
        }
        m_downFileUrlInfoList.pop_front();
        if(!m_downFileUrlInfoList.isEmpty())
        {
          downFile(m_downFileUrlInfoList[0].name());
        }
        else//下载完毕
        {
            emit downLoadFinshed();
        }

        break;

    case QFtp::Put:
        qDebug()<<"upload finshed";

        if(m_file !=NULL && m_file->isOpen())
        {
            m_file->close();
            m_file->deleteLater();;
            m_file=NULL;
        }
        m_uploadFileList.pop_front();
        if(!m_uploadFileList.isEmpty())
        {
            QString _headStr(QApplication::applicationDirPath()+"/" );
            int _headIndex=  m_uploadFileList[0].indexOf(_headStr);
            QString _remotePath=m_uploadFileList[0].mid(_headIndex+_headStr.size());//目前只有一层 比如 a\\b.txt

            putFile(m_uploadFileList[0],_remotePath);

        }

        // emit uploadFinshed();

        break;

    case QFtp::Mkdir:

        if(!error)
        {
            emit mkdirSucceed();
            m_flag=true;
        }

        break;
    default:
        break;
    }

    if(error)//有错误
    {
        qDebug()<<"error:"<<m_ftp->errorString();
    }
}

void ObjectFtp::listInfoSlot(QUrlInfo _urlInfo)
{
    qDebug()<<_urlInfo.name();

    if(_urlInfo.isDir())//dir
    {
        m_DirurlInfoList<<_urlInfo;//目录
    }
    m_urlInfoList<<_urlInfo;
}


void ObjectFtp::uploadDataTransferProgressSlot(qint64 value,qint64 total)
{
    emit progressValue( value, total);
}
