#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore/QCoreApplication>
#include "zlib.h"
#include "stdio.h"
#include "qdebug.h"
#include "qfile.h"
#define MaxBufferSize 999999
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    FILE* File_src;
    FILE* File_compress;
    FILE* File_uncompress;

    unsigned long len_src;
    unsigned long len_compress;
    unsigned long len_uncompress = MaxBufferSize;

    unsigned char *buffer_src  = new unsigned char[MaxBufferSize];
    unsigned char *buffer_compress  = new unsigned char[MaxBufferSize];
    unsigned char *buffer_uncompress = new unsigned char[MaxBufferSize];

    File_src = fopen("src.txt","r");
    File_compress = fopen("compress.txt","w");
    File_uncompress = fopen("uncompress.txt","w");

    //compress
    len_src = fread(buffer_src,sizeof(char),MaxBufferSize-1,File_src);
    compress(buffer_compress,&len_compress,buffer_src,len_src);
    fwrite(buffer_compress,sizeof(char),len_compress,File_compress);
    qDebug() << "normal zlib:" ;
    qDebug() << "src:\n" << buffer_src << ",length:" << len_src ;
    qDebug() << "compress:\n" << buffer_compress << ",length:" << len_compress ;

    //uncompress
    uncompress(buffer_uncompress,&len_uncompress,buffer_compress,len_compress);
    fwrite(buffer_uncompress,sizeof(char),len_uncompress,File_uncompress);
    qDebug() << "uncompress:\n" << buffer_uncompress << ",length:" << len_uncompress;

    fclose(File_src);
    fclose(File_compress);
    fclose(File_uncompress);
}

void MainWindow::on_pushButton_2_clicked()
{
    QByteArray src;
    src.append("中华人民共和国,china mobile,123456 ");

    unsigned long len_compress;
    unsigned long len_uncompress;

    unsigned char *buffer_compress  = new unsigned char[MaxBufferSize];
    unsigned char *buffer_uncompress = new unsigned char[MaxBufferSize];

    compress(buffer_compress,&len_compress,(Bytef*)src.data(), src.length());
    uncompress(buffer_uncompress,&len_uncompress,buffer_compress,len_compress);

    qDebug() << "qt zlib:"  ;
    qDebug() << "src:\n" << src.data() << ",length:" << src.size() ;
    qDebug() << "compress:\n" << buffer_compress << ",length:" << len_compress ;
    qDebug() << "uncompress:\n" << buffer_uncompress << ",length:" << len_uncompress ;
}

//this is noe test
void MainWindow::on_pushButton_3_clicked()
{/*
    unz_file_info64 FileInfo;
    unzFile zFile = unzOpen64("d:/biji.zip");
    if(NULL == zFile)
        qDebug() << "zero!";
    else
        qDebug() << "haha";

    unz_global_info64 gi;

    if (unzGetGlobalInfo64(zFile, &gi) != UNZ_OK)
    {
        qDebug() << "ho no";
    }

    int result;
    for (int i = 0; i < gi.number_entry; ++i)
    {
        char file[256] = {0};
        char ext[256] = {0};

        char com[1024] = {0};
        if (unzGetCurrentFileInfo64(zFile, &FileInfo,file,sizeof(file),ext,256,com,1024)!= UNZ_OK)
            qDebug() << "error";
        qDebug() << file << ":" << ext << ":" << com;



        if(!(FileInfo.external_fa & FILE_ATTRIBUTE_DIRECTORY)) //文件，否则为目录

        //打开文件
         result=unzOpenCurrentFile(zFile);//无密码
        result=unzOpenCurrentFilePassword(zFile,"szPassword"); //有密码

        char data[1024] = {0};
        int size;
        //读取内容

        QString path = QString("d:/zip/") + file;
        QFile f(path);
        f.open(QFile::WriteOnly);
        while(true) {
            size= unzReadCurrentFile(zFile,data,sizeof(data));
            if(size <= 0)
                break;

    //            QString str;
    //            str = QString::fromLocal8Bit(data, size);
            f.write(data, size);
        }
        f.close();

        //关闭当前文件
        unzCloseCurrentFile(zFile);

        //出错
        if(i < gi.number_entry - 1 && unzGoToNextFile(zFile) != UNZ_OK)
            qDebug() << "error2";
    }
    unzClose(zFile);*/
}
