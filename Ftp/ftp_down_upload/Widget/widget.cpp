#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QProcess>
#include <windows.h>
extern bool g_flag;
widget::widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widget)
{
    qRegisterMetaType<QUrlInfo>("QUrlInfo");
    ui->setupUi(this);


    m_downFtp= new ObjectFtp();
    //m_downFtp->initFtpClient(QString("ftp"),QString("ftp"),QString("10.0.0.232"));
     m_downFtp->initFtpClient(QString("wqiankun"),QString("123456"),QString("10.0.0.249"));
    connect(m_downFtp,SIGNAL(listUrlInfoList(QList<QUrlInfo>)),this,SLOT(downListinfoListSlot(QList<QUrlInfo>)));//list传过来的文件夹
    connect(m_downFtp,SIGNAL(progressValue(qint64,qint64)),this,SLOT(  downProgressValueSlot(qint64,qint64)));//进度
    connect(m_downFtp,SIGNAL(downLoadFinshed()),this,SLOT(downFinshed()));//所有的文件都 下载完成


    m_workFtp =new ObjectFtp();
   // m_workFtp->initFtpClient(QString("wqiankun"),QString("123456"),QString("10.0.0.249"));
    m_workFtp->initFtpClient(QString("ftp"),QString("ftp"),QString("10.0.0.232"));
    connect(m_workFtp,SIGNAL(listUrlInfoList(QList<QUrlInfo>)),this,SLOT(listinfoListSlot(QList<QUrlInfo>)));//list传过来的文件夹
    connect(m_workFtp,SIGNAL(progressValue(qint64,qint64)),this,SLOT(  progressValueSlot(qint64,qint64)));//进度
    connect(m_workFtp,SIGNAL(uploadFinshed()),this,SLOT(uploadFinshed()));//上传完成


//     m_workFtp->listDir("zwt");
//      m_workFtp->listDir("testDir");
/*
    QString _localStr("E:\\LSwuqiankun\\ftp_down_upload\\debug\\op\\name.exe");
    QString _remote("mio\\feiqiu.exe");
    //  m_workFtp->putFile(_localStr,_remote);

   m_remoteUrlInfoList.clear();
    m_workFtp->listDir("zwt");

    int i=0;
    int _timeout=0;
    while(g_flag== false || _timeout<10)
    {
        qApp->processEvents();
         // g_flag=true;
        Sleep(100);
        _timeout++;
    }


    bool _isMK=false;
    qDebug()<<"hhhhhhhhh"<<m_remoteUrlInfoList.count();
    for(int i=0;i<m_remoteUrlInfoList.size();++i)
    {
        qDebug()<<"name"<<m_remoteUrlInfoList[i].name();
        if(m_remoteUrlInfoList[i].name()=="b00012")
        {
            _isMK==true;
            break;
        }
    }

    if(_isMK==false)
    {
        int i=0;
        int _timeout=0;
        while(g_flag== false ||_timeout<10)
        {
            qApp->processEvents();
             // g_flag=true;
            Sleep(100);
            _timeout++;
        }
        m_workFtp->mkDir(QString("zwt/b00012"));
    }
*/

/*
      _isMK=false;
      m_remoteUrlInfoList.clear();
     m_workFtp->listDir("testDir");
     for(int i=0;i<m_remoteUrlInfoList.size();++i)
     {
          qDebug()<<m_remoteUrlInfoList[i].name();
         if(m_remoteUrlInfoList[i].name()=="b00013")
         {
             _isMK==true;
             break;
         }
     }


     if(_isMK==false)
     {
         m_workFtp->mkDir("b00013");

     }
     */

    m_downServerDownDir="2017_01_16";
    m_downFtp->downDir(m_downServerDownDir);


    //this is ok
    // m_workFtp->putFileList(getLocalFileList(QApplication::applicationDirPath()+"/wqiankun"));
}

#include <QDir>
#include <QFileInfo>
QStringList   widget::getLocalFileList(QString dirStr)
{
    QDir _dir(dirStr);
   QStringList _fileList= _dir.entryList(QDir::Files);

   for(int i=0;i<_fileList.count();++i)
   {
       _fileList[i]=dirStr+"/"+ _fileList[i];
   }
   qDebug()<<"fileList:"<<_fileList;

   return _fileList;

}

widget::~widget()
{
    delete ui;
}
void widget::progressValueSlot(qint64 _value,qint64 _total)
{
    qDebug()<<_value <<_total;
}
void widget::downProgressValueSlot(qint64 _value,qint64 _total)
{

    qDebug()<<_value <<_total;
}


void widget::listinfoListSlot(QList<QUrlInfo> _urlInfoList)
{
  m_remoteUrlInfoList=_urlInfoList;
}

void widget::downListinfoListSlot(QList<QUrlInfo> _urlInfoList)
{
  m_downServerUrlInfoList=_urlInfoList;
  //可以在开一个定时器，看看 文件是否为空，如果不为空，就下载文件

}



void widget::uploadFinshed()
{

}


/*
函数名：所有的文件都下载完毕
*/
void widget::downFinshed()
{
  m_workFtp->putFileList(getLocalFileList(QApplication::applicationDirPath()+"/2017_01_16"));
}
