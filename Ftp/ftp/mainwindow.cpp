#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    manager = new FtpManager("10.0.0.249","ftp","ftp",21,this);
    manager2 = new FtpManager("10.0.0.249","ftp","ftp",21,this);
    connect(manager,SIGNAL(G_getProgressVal(int)),SLOT(S_updateProgess(int)));
    connect(manager2,SIGNAL(G_getProgressVal(int)),SLOT(S_updateProgess2(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//更新进度条

void MainWindow::S_updateProgess(int _val)
{

    ui->progressBar->setValue(_val);
}


//更新进度条
void MainWindow::S_updateProgess2(int _val)
{

    ui->progressBar_2->setValue(_val);
}


//普通下载
void MainWindow::on_downloadBn_clicked()
{
    manager->S_dloadFile("续传","D:/QTWorkspace.zip");
}

//普通上传
void MainWindow::on_uploadBn_clicked()
{
    manager->S_uloadFile(QString("D:/压缩包/testFtp.zip"),"/三/test.zip");
}

//下载（续传）
void MainWindow::on_downloadBn_2_clicked()
{
    manager2->S_dloadFile("续传","D:/testFtp.zip",true);
}

//停止
void MainWindow::on_abort_clicked()
{
    manager2->S_abort();
}

//上传（续传）
void MainWindow::on_abort_2_clicked()
{
    manager2->S_uloadFile("D:/压缩包/三.zip",( QString("/三/三.zip")) ,true);
}


//停止
void MainWindow::on_abort_3_clicked()
{
    manager->S_abort();
}

//显示文件列表
void MainWindow::on_downloadBn_3_clicked()
{
    manager->S_list();
}

void MainWindow::on_pushButton_clicked()
{
   manager->deleteFile(QString("/三/三.zip"));
}
