#include "savefile.h"
#include <QDebug>
#define TimeInterval  100000
SaveFile::SaveFile(QString fileParentDir,QObject *parent) :
    QObject(parent),m_dirPath(fileParentDir)
{
  initFile();
}


SaveFile::~SaveFile()
{
    if(m_timerInterval !=NULL)
    {
        m_timerInterval->deleteLater();
        m_timerInterval=NULL;
    }
    if(m_file!=NULL)
    {
        m_file->deleteLater();
        m_file=NULL;
    }
}
void SaveFile::initFile()
{
    QString _fileName=m_dirPath+"/"+QTime::currentTime().toString()+"/"+"log.txt";
    m_file= new QFile(_fileName);
    m_fout.setDevice(m_file);

    m_timerInterval= new QTimer(this);
    QObject::connect(m_timerInterval,SIGNAL(timeout()),this,SLOT(changeFile()));

    m_timerInterval->setInterval(TimeInterval);
}

void SaveFile::changeFile()
{
    if(m_file->isOpen())
    {
        m_file->close();
        m_file->deleteLater();
        m_file= NULL;
    }
    QString _fileName=m_dirPath+"/"+QTime::currentTime().toString()+"/"+"log.txt";
    m_file= new QFile(_fileName);
    if(m_file->open(QFile::ReadWrite | QFile::Truncate))
    {
        qDebug()<<"not openMode";
        return;
    }
    m_fout.setDevice(m_file);
}

void SaveFile::writeInFile(QStringList dataList )
{
    if(NULL == m_file)
    {
        QString _fileName=m_dirPath+"/"+QTime::currentTime().toString()+"/"+"log.txt";
        m_file= new QFile(_fileName);
    }
    if(m_file->isOpen())
    {
        if(m_file->open(QFile::ReadWrite | QFile::Truncate))
        {
            qDebug()<<"not openMode";
            return;
        }
    }
    m_fout.setDevice(m_file);

    int i=0;
    while(i<dataList.count())
    {
         m_fout<<dataList[i]<<"\n";
         ++i;
    }

}
