#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <windows.h>

//QSemaphore g_seamphore(1);
//QSemaphore g_mkSeamphore(1);
bool g_mkBool=false;
bool g_listBool=false;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),m_file(NULL),m_uplodFile(NULL),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    m_saveLocalPath=QApplication::applicationDirPath()+"/";

    m_udpSocket= new QUdpSocket(this);
    m_udpSocket->bind(45454,QUdpSocket::ShareAddress);
    connect(m_udpSocket,SIGNAL(readyRead()),this,SLOT(processPendingDatagrams()));




    m_ftp =new Ftp();
    m_ftp->initFtpClient(QString("wqiankun"),QString("123456"),QString("10.0.0.249"));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(downFile()));
    //m_ftp->list("testDir/test");

    connect(m_ftp,SIGNAL(commandStarted(int)),this,SLOT(commandStartedSlot(int)));
    connect(m_ftp,SIGNAL(commandFinished(int,bool)),this,SLOT(commandFinshed(int ,bool )));
    // connect(m_ftp,SIGNAL(rawCommandReply(int,QString))
    connect(m_ftp,SIGNAL(listInfo(QUrlInfo)),this,SLOT(listInfoSlot(QUrlInfo )));

    connect(m_ftp,SIGNAL(dataTransferProgress(qint64,qint64)),this,SLOT( dataTransferProgressSlot(qint64,qint64)));
    // connect(m_ftp,SIGNAL(done(bool)),this,SLOT(  finshedSlot(bool)));
    //  m_ftp->get("ftpFile.txt",m_file);


    m_uploadFtp= new Ftp();
    // m_uploadFtp->initFtpClient(QString("wqiankun"),QString("123456"),QString("10.0.0.249"));
    m_uploadFtp->initFtpClient(QString("FTP"),QString("FTP"),QString("10.0.0.232"));
    connect(ui->uploadBtn,SIGNAL(clicked()),this,SLOT(sendNewCommand()));
    connect(m_uploadFtp,SIGNAL(dataTransferProgress(qint64,qint64)),this,SLOT( uploadDataTransferProgressSlot(qint64,qint64)));
    //connect(m_uploadFtp,SIGNAL(done(bool)),this,SLOT(  finshedSlot(bool)));
    connect(m_uploadFtp,SIGNAL(stateChanged(int)),this,SLOT(stateChangeSlot(int)));
    connect(m_uploadFtp,SIGNAL(listInfo(QUrlInfo)),this,SLOT(uploadServerlistInfoSlot(QUrlInfo )));
    connect(m_uploadFtp,SIGNAL(commandStarted(int)),this,SLOT(uploadCommandStart(int)));

    connect(m_uploadFtp,SIGNAL(commandFinished(int,bool)),this,SLOT(uploadCommandFinshed(int ,bool )));


    m_remotePath="zwt";
    if(!m_remotePath.isEmpty())
    {
        m_ftp->list(m_remotePath);
    }

}


void MainWindow::sendNewCommand()
{
    qDebug()<<"sendNewCommand";
    m_uploadFtp->errorString();
    m_uploadFtp->list("a");
}

/*
函数名：接收服务器发来的远程服务器的下载文件路径
wuqiankun
*/
void MainWindow::processPendingDatagrams()
{
    while(m_udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        m_udpSocket->readDatagram(datagram.data(),datagram.size());
        //m_remotePath(QString(datagram.data()));

        //test
        m_remotePath="testDir";

        if(!m_remotePath.isEmpty())
        {
            m_ftp->list(m_remotePath);
        }
    }
}



void MainWindow::commandStartedSlot(int _commandIndex)
{
    qDebug()<<"_commandIndex:"<<".........."<<_commandIndex;
}

void MainWindow::commandFinshed(int _index,bool ff)
{
    qDebug()<<"commandFinshed:"<<".........."<<_index<<ff<<m_ftp->currentCommand();
    if(m_ftp->currentCommand()==QFtp::List)//list() 结束
    {

        qDebug()<<"-------------------------------";
        if(!m_urlInfoList.isEmpty())
        {
            if( m_urlInfoList[0].isFile())//只针对文件
            {
            }
            QString remotePah= m_remotePath+"/"+ m_urlInfoList[0].name();
            downFile(remotePah );
        }
    }

}

void MainWindow::uploadCommandStart(int)
{
    qDebug()<<"uoload"<<m_uploadFtp->currentCommand();
}

void MainWindow::uploadCommandFinshed(int _index ,bool _flag)
{
    qDebug()<<" uploadCommandFinshed........."<<m_uploadFtp->currentCommand()<<m_uploadFtp->errorString();
    if((m_uploadFtp->currentCommand()== QFtp::List) && (m_opIndex== _index))
    {
        qDebug()<<"release resource........."<<_flag;
        // g_seamphore.release();
        g_listBool=true;

    }
    else if(m_uploadFtp->currentCommand()== QFtp::Mkdir)
    {
        //g_mkSeamphore.release();
        g_mkBool=true;
    }
  return;

}

void MainWindow::listInfoSlot(QUrlInfo _info)
{
    // qDebug()<<_info.name();
    m_urlInfoList<<_info;
}

//远程服务B
void MainWindow::uploadServerlistInfoSlot(QUrlInfo _urlinfo)
{
    qDebug()<<_urlinfo.name();
    if(_urlinfo.isDir())
    {
        // qDebug()<<_urlinfo.name();
        m_remoteDirList<<_urlinfo.name();
    }

}

/*
函数名：可以创建a/b/c
*/
void MainWindow::mkRemoteDir(QString remoteDir)
{
    m_remoteDirList.clear();
    QStringList _dirList=remoteDir.split("\\");

    /*

    qDebug()<<"可利用的信号量："<<  g_seamphore.available();

     g_seamphore.acquire();
     m_uploadFtp->list();
    qDebug()<<"可利用的信号量222："<<  g_seamphore.available();
   bool _ret=false;
   qDebug()<<"可利用的信号量22211111111111："<<  g_seamphore.available();
   while(_ret== false)
   {
       qDebug()<<"可利用的信号量2221113333333333333："<<  g_seamphore.available();
        qApp->processEvents(); //
       _ret= g_seamphore.tryAcquire(1,100);
       qDebug()<<"可利用的信号量22211144444444444444："<<  g_seamphore.available();
       qDebug()<<m_uploadFtp->state();
   }

   qDebug()<<"可利用的信号量3333222222222222222222："<<  g_seamphore.available();
   g_seamphore.release();
    qDebug()<<"可利用的信号量3333："<<  g_seamphore.available();
qDebug()<<"文件目录列表："<<m_remoteDirList;
*/

    int i=0;
    QString _str("");
    qDebug()<<"需要建立的远程目录："<<_dirList<<_dirList.size();
    while(i<_dirList.count())
    {

       m_opIndex= m_uploadFtp->list(_str);
        qDebug()<<"g_listBool"<<g_listBool<<_str<<m_uploadFtp->state();
        int _timeout=0;
        while(g_listBool==false)
        {
            qDebug()<<"iiiiiiiiiiiiiiiiiiiiiii";
            qApp->processEvents(); //
            //Sleep(1000);
            _timeout++;
            if(_timeout> 20)
            {
               break;
            }
        }
        g_listBool=false;



        qDebug()<<g_mkBool<<_str<<_dirList[i]<<"文件目录列表："<<m_remoteDirList;

        if(!m_remoteDirList.contains(_dirList[i]))
        {
            qDebug()<<"enter........";
            m_uploadFtp->mkdir(_dirList[i]);
            int _timtOutCd=0;
            while(g_mkBool ==false)
            {
               qApp->processEvents(); //
               // Sleep(1000);

                if(_timtOutCd> 10)
                {
                   break;
                }
            }
            g_mkBool=false;

        }
        qDebug()<<"list----"<<_str;
        if(i!= 0)
        {
            _str+="\\";
        }

        // m_uploadFtp->mkdir(_str);

        _str+=_dirList[i];

        i++;
    }
}

void MainWindow::stateChangeSlot(int _stateCode)
{
    qDebug()<<"状态发生变换"<<_stateCode;
}

MainWindow::~MainWindow()
{
    if(m_file != NULL)
    {
        m_file->deleteLater();
        m_file=NULL;
    }

    if(m_udpSocket != NULL)
    {
        m_udpSocket->deleteLater();
        m_udpSocket=NULL;
    }

    if(m_ftp != NULL)
    {
        m_ftp->close();
        m_ftp->deleteLater();
        m_ftp=NULL;
    }


    if(m_uploadFtp != NULL)
    {
        m_uploadFtp->close();
        m_uploadFtp->deleteLater();
        m_uploadFtp=NULL;
    }
    delete ui;
}
void MainWindow::downFile(QString remoteFileName)
{
    qDebug()<<m_ftp->errorString();
    if(QFile::exists(remoteFileName))
    {
        QFile::remove(remoteFileName);
    }
    QDir _dir(m_saveLocalPath+m_remotePath);
    if( !_dir.exists() )
    {
        _dir.mkpath(m_saveLocalPath+m_remotePath);
    }


    // qDebug()<<"filepath:------"<<QString(m_saveLocalPath+remoteFileName).replace("/","\\");
    m_file= new QFile( QString(m_saveLocalPath+remoteFileName));

    if (!m_file->open(QIODevice::ReadWrite ))
    {
        //  qDebug()<<"not....";
        return;
    }


    m_ftp->get(remoteFileName,m_file);
}


void MainWindow::dataTransferProgressSlot(qint64 date,qint64 total)
{
    // qDebug()<<date<<"  "<<total;
    if(date >= total)
    {
        m_fileList.push_back(m_file->fileName());

        //qDebug()<<"fINSHED.11111.....";//here has a Qt bug : when upload file show QIOdevice::read    device not open
        if((m_file!=NULL) && (m_file->isOpen()))
        {
            m_file->close();
            m_file->deleteLater();
            m_file=NULL;
        }


        m_urlInfoList.pop_front();
        if(!m_urlInfoList.isEmpty())
        {
            //下载文件
            QString remotePah= m_remotePath+"/"+ m_urlInfoList[0].name();
            downFile(remotePah);
        }
        else
        {
            //上传文件
            qDebug()<<"enter upload FFFFFFFFFFFFFF"<<m_fileList;

            uploadFile(m_fileList[0]);

        }

        // qDebug()<<"fINSHED.22222.....";
    }
}

/*
函数名：上传文件的进度条

注意：如果远程目录已经存在了，再创建出现错误。 下面的代码不执行了
*/
void MainWindow::uploadDataTransferProgressSlot(qint64 date,qint64 total)
{
    qDebug()<<"===========uuuuuuuuuuuuuuu============================"<<date<<total;
    if(date >= total)
    {
        //上传文件
        m_fileList.pop_front();
        qDebug()<<"uipload fileList:"<<m_fileList;
        if(!m_fileList.isEmpty())
        {
            qDebug()<<"list---------------"<<m_fileList;


            uploadFile(m_fileList[0]);
        }

        qDebug()<<"uploadDataTransferProgressSlot.22222.....";
    }
}

void MainWindow::uploadFile(QString localFileName)
{
    // qDebug()<<m_uploadFtp->errorString();


    m_uplodFile= new QFile(localFileName);
    if( !m_uplodFile->open(QIODevice::ReadWrite))
    {
        qDebug()<<"not";
    }
    QString _fileDirPath=localFileName.remove( (QApplication::applicationDirPath()+"/") );
    int _index=_fileDirPath.lastIndexOf("/");
    if(_index != -1)
    {
        //新建文件夹
        QString _remoteDir= _fileDirPath.left(_index);
        qDebug()<<"_remoteDir"<<_remoteDir;
        mkRemoteDir(_remoteDir);
        // qDebug()<<"error--------------"<<m_uploadFtp->errorString()<<m_uploadFtp->currentCommand();
    }
    //  _index=_fileDirPath.lastIndexOf("/");
    // _fileDirPath=_fileDirPath.mid(_index+1);//fileName

    qDebug()<<m_uplodFile->fileName()<<_fileDirPath;
    // m_uploadFtp->putFile(m_uplodFile,"n");
    m_uploadFtp->putFile(m_uplodFile,_fileDirPath);
}

void MainWindow::finshedSlot(bool _flage )
{
    qDebug()<<m_uploadFtp->errorString();
}


QStringList MainWindow::getfileList(QDir *_dir)
{

}
