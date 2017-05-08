#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QProcess>
#include <windows.h>
#include <QRegExp>
#include <QMessageBox>
extern bool g_flag;
widget::widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widget)
{
    qRegisterMetaType<QUrlInfo>("QUrlInfo");
    ui->setupUi(this);
    initUdpSocket();

    downLoadFtpInit();
    uploadFtpInit();
    // m_workFtp->mkDir("/b00013");//不能直接创建嵌套的文件夹 /a/b 如果a文件夹不存在，执行这条Ftp命令是无效的
    //m_workFtp->mkDir("/b00013/A");

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

    //test
    m_downServerDownDir="2017_01_16";
    // m_downFtp->downDir(m_downServerDownDir);

    //m_workFtp->putFileList(getLocalFileList(QApplication::applicationDirPath()+"/"+m_downServerDownDir));

    //m_downFtp->rawCommand("ftpFile.txt");

    //this is ok
    // m_workFtp->putFileList(getLocalFileList(QApplication::applicationDirPath()+"/wqiankun"));
}

void widget::downLoadFtpInit()
{
    m_downFtp= new ObjectFtp();
    //m_downFtp->initFtpClient(QString("ftp"),QString("ftp"),QString("10.0.0.232"));
    m_downFtp->initFtpClient(QString("wqiankun"),QString("123456"),QString("10.0.0.249"));
    connect(m_downFtp,SIGNAL(listUrlInfoList(QList<QUrlInfo>)),this,SLOT(downListinfoListSlot(QList<QUrlInfo>)));//list传过来的文件夹
    connect(m_downFtp,SIGNAL(progressValue(qint64,qint64)),this,SLOT(  downProgressValueSlot(qint64,qint64)));//进度
    connect(m_downFtp,SIGNAL(downLoadFinshed()),this,SLOT(downFinshed()));//所有的文件都 下载完成
    connect(m_downFtp,SIGNAL(downProress(int,int)),this,SLOT(downFileProgressValue(int,int)));//文件传输个数进度
}


void widget::uploadFtpInit()
{
    m_workFtp =new ObjectFtp();
    // m_workFtp->initFtpClient(QString("wqiankun"),QString("123456"),QString("10.0.0.249"));
    m_workFtp->initFtpClient(QString("wqiankun"),QString("123456"),QString("10.0.0.249"));
    connect(m_workFtp,SIGNAL(listUrlInfoList(QList<QUrlInfo>)),this,SLOT(listinfoListSlot(QList<QUrlInfo>)));//list传过来的文件夹
    connect(m_workFtp,SIGNAL(progressValue(qint64,qint64)),this,SLOT(  progressValueSlot(qint64,qint64)));//进度
    connect(m_workFtp,SIGNAL(uploadFinshed()),this,SLOT(uploadFinshed()));//上传完成
    connect(m_workFtp,SIGNAL(uploadProgress(int,int)),this,SLOT(uploadFileProgressValue(int,int)));//文件传输个数进度

}


void widget::initUdpSocket()
{
    m_udpSocket= new QUdpSocket(this);
    m_udpSocket->bind(45454,QUdpSocket::ShareAddress);
    connect(m_udpSocket,SIGNAL(readyRead()),this,SLOT(processPendingDatagrams()));
}


void widget::uploadIpLineEdit()
{
    ui->iplineEditOne_2->setMaxLength(3);
    ui->iplineEditTwo_2->setMaxLength(3);
    ui->iplineEditThree_2->setMaxLength(3);
    ui->iplineEditFour_2->setMaxLength(3);

    QRegExp rx1("^(2[0-4][0-9]|25[0-5]|1[0-9]{2}|[1-9][0-9]|[1-9])$");
    ui->iplineEditOne_2->setValidator(new QRegExpValidator(rx1, this));

    QRegExp rx2("^(2[0-4][0-9]|25[0-5]|1[0-9]{2}|[1-9][0-9]|[0-9])$");
    ui->iplineEditTwo_2->setValidator(new QRegExpValidator(rx2, this));
    ui->iplineEditThree_2->setValidator(new QRegExpValidator(rx2, this));
    ui->iplineEditFour_2->setValidator(new QRegExpValidator(rx2, this));
}

QString widget::getUpLoadIpStr()const
{
    QString ippart1,ippart2,ippart3,ippart4;
    ippart1 = ui->iplineEditOne_2->text();
    ippart2 = ui->iplineEditTwo_2->text();
    ippart3 = ui->iplineEditThree_2->text();
    ippart4 = ui->iplineEditFour_2->text();
    if(ippart1.isEmpty() && ippart2.isEmpty() &&ippart3.isEmpty() && ippart4.isEmpty())
    {
        // QMessageBox::information( (QWidget*)this,"提示：","请查看“IP地址”是否填写完毕","确定");
        return QString();
    }

    return QString("%1.%2.%3.%4")
            .arg(ippart1)
            .arg(ippart2)
            .arg(ippart3)
            .arg(ippart4);
}

void widget::downIpLineEdit()
{
    ui->iplineEditOne->setMaxLength(3);
    ui->iplineEditTwo->setMaxLength(3);
    ui->iplineEditThree->setMaxLength(3);
    ui->iplineEditFour->setMaxLength(3);

    QRegExp rx1("^(2[0-4][0-9]|25[0-5]|1[0-9]{2}|[1-9][0-9]|[1-9])$");
    ui->iplineEditOne->setValidator(new QRegExpValidator(rx1, this));

    QRegExp rx2("^(2[0-4][0-9]|25[0-5]|1[0-9]{2}|[1-9][0-9]|[0-9])$");
    ui->iplineEditTwo->setValidator(new QRegExpValidator(rx2, this));
    ui->iplineEditThree->setValidator(new QRegExpValidator(rx2, this));
    ui->iplineEditFour->setValidator(new QRegExpValidator(rx2, this));
}


QString widget::getDownLoadIpStr()const
{
    QString ippart1,ippart2,ippart3,ippart4;
    ippart1 = ui->iplineEditOne->text();
    ippart2 = ui->iplineEditTwo->text();
    ippart3 = ui->iplineEditThree->text();
    ippart4 = ui->iplineEditFour->text();
    if(ippart1.isEmpty() && ippart2.isEmpty() &&ippart3.isEmpty() && ippart4.isEmpty())
    {
        //QMessageBox::information( (QWidget*)this,"提示：","<p><font color=black>请查看“IP地址”是否填写完毕<font/><p/>","确定");
        return QString();
    }

    return QString("%1.%2.%3.%4")
            .arg(ippart1)
            .arg(ippart2)
            .arg(ippart3)
            .arg(ippart4);
}

/*
函数名：发送端 发送的汉子是 UTF8编码格式，这样接受的汉子才能正确显示
*/
void widget::processPendingDatagrams()
{
    // m_udpSocket->readAll();
    while(m_udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        m_udpSocket->readDatagram(datagram.data(),datagram.size());
        //m_downServerDownDir(QString(datagram.data()));

        qDebug()<<QString::fromUtf8(datagram);
        //test
        m_downServerDownDir="testDir";

        if(!m_downServerDownDir.isEmpty())
        {
            // m_downFtp->downDir(m_downServerDownDir);
        }
    }
}

/*
函数名：下载文件个数进度
*/
void widget::downFileProgressValue(int curFileCount,int totalFileCount)
{
    qDebug()<<"down FileProgressValue:"<<curFileCount<<totalFileCount;
    ui->downTotalProgressBar->setMaximum(totalFileCount);
    ui->downTotalProgressBar->setValue(curFileCount);
}

/*
函数名：上传文件个数进度
*/
void widget::uploadFileProgressValue(int curFileCount,int totalFileCount)
{
    qDebug()<<"upload FileProgressValue:"<<curFileCount<<totalFileCount;
    ui->uploadTotalProgressBar->setMaximum(totalFileCount);
    ui->uploadTotalProgressBar->setValue(curFileCount);
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
    // m_downFtp->deleteLater();
    // m_workFtp->deleteLater();
    delete m_downFtp;
    delete m_workFtp;
    delete ui;
}
void widget::progressValueSlot(qint64 _value,qint64 _total)
{
    //qDebug()<<"upload:"<<_value <<_total<<(_value/_total);
    ui->uploadProgressBar->setMaximum(_total);
    ui->uploadProgressBar->setValue(_value);
}
void widget::downProgressValueSlot(qint64 _value,qint64 _total)
{
    // qDebug()<<"download:"<<_value <<_total<<(_value/_total);
    ui->downProgressBar->setMaximum(_total);
    ui->downProgressBar->setValue(_value);
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
    //qDebug()<<"文件上传完毕。";

    qDebug()<<m_downFtp->m_ftp->state()<<m_workFtp->m_ftp->state();
    m_downFtp->disconnect();
    m_downFtp->m_ftp->abort();
    m_downFtp->m_ftp->close();
    m_downFtp->deleteLater();
    //delete m_downFtp;
    m_downFtp=NULL;


    m_workFtp->disconnect();
    m_workFtp->m_ftp->abort();
    m_workFtp->m_ftp->close();
    m_workFtp->deleteLater();
    //delete m_workFtp;
    m_workFtp=NULL;

    downLoadFtpInit();
    uploadFtpInit();

    m_downServerDownDir="2017_01_16";
    m_downFtp->downDir(m_downServerDownDir);

    //m_workFtp->putFileList(getLocalFileList(QApplication::applicationDirPath()+"/"+m_downServerDownDir));
    qDebug()<<"++++++++++++++++++++";

}


/*
函数名：所有的文件都下载完毕
*/
void widget::downFinshed()
{
    //所有文件已经下载完成


    //    m_downFtp->disconnect();
    //    m_downFtp->m_ftp->abort();
    //    m_downFtp->m_ftp->close();
    //    delete m_downFtp;

    //    downLoadFtpInit();

    //    m_downServerDownDir="2017_01_16";
    //    m_downFtp->downDir(m_downServerDownDir);

    //上传
    m_workFtp->putFileList(getLocalFileList(QApplication::applicationDirPath()+"/"+m_downServerDownDir));
}
