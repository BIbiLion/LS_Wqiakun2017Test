#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ftp.setTransferMode(QFtp::Passive);
    ftp.connectToHost("10.0.0.249");
    ftp.login("wqiankun","123456");
    ftp.rawCommand("SIZE ftpFile.txt");//得到文件大小
    ftp.get("ftpFile.txt",nullptr,500);
    connect(&ftp,&QFtp::rawCommandReply,[this](int ,const QString & str){
        this->ui->textBrowser->append(str);
    });//执行命令的结果
    connect(&ftp,&QFtp::readyRead,[this](){
        this->ui->textBrowser->append(this->ftp.readAll());
    });//显示下载的内容


    upload_test();
}

MainWindow::~MainWindow()
{
    delete ui;
}


//以此来实现断点续传
#include <QFile>
#include <QTextBrowser>
 void MainWindow::upload_test()
 {
     //this is a test 修改文件的位置
//   QFile _file("F:\\FtpWorkspace\\ftpFile.txt");
//   _file.open(QIODevice::ReadWrite);
//   _file.seek(500);
//   QTextBrowser * _brower= new QTextBrowser();
//   _brower->append( _file.readAll());
//   _brower->show();
  //  _file.close();

    QFile *  _file= new QFile("F:\\FtpWorkspace\\ftpFile.txt");
    _file->open(QIODevice::ReadWrite);
   _file->seek(500);


    ftp.put( _file->readAll(),QString("testVertify.txt"));
    _file->close();
 }
