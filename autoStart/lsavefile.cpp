#include "lsavefile.h"



LSaveFile::LSaveFile(QString basePath, unsigned int itemCount, QStringList& itemNames, unsigned int fileTimerVar)
    :m_itemName(itemNames)
{
    m_basePath=basePath;
    m_itemCount=itemCount;
    m_dataFile=new QFile*[m_itemCount];
    for(unsigned int i=0;i<m_itemCount;i++)
    {
        m_dataFile[i]=NULL;
    }
    m_fileTimerVar=fileTimerVar;
    m_fileTimer=new QTimer();
    m_fileTimer->setInterval(m_fileTimerVar);
    QObject::connect(m_fileTimer,SIGNAL(timeout()),this,SLOT(changeFile()));
    changeFile();
    //m_writeTimer=writeTimerVar;
    //m_writeTimer=new QTimer();
    //connect(m_writeTimer,SIGNAL(timeout()),this,SLOT(changeFile()));

}

LSaveFile::~LSaveFile()
{
    for(unsigned int i=0;i<m_itemCount;i++)
    {
        m_dataFile[i]->close();
    }
    delete[] m_dataFile;
    delete m_fileTimer;
    //delete m_writeTimer;
}


QFile* LSaveFile::getDataFile(unsigned int i)
{
    return m_dataFile[i];
}

void LSaveFile::writeFile(unsigned int index,QStringList data)
{
    if (!m_dataFile[index]->isOpen()){m_dataFile[index]->open(QFile::WriteOnly | QFile::Truncate);}// may have security problem when file is unavaliable as program begins
    fout.setDevice(m_dataFile[index]);
    unsigned int length = data.count();
    for(unsigned int i=0;i<length;i++)
    {
        fout<<data[i];
    }
    //fout.flush();
}
//0 receive  ; 1 send
void LSaveFile::writeFileBinary(unsigned int index,QByteArray data,bool direction)
{
    if (!m_dataFile[index]->isOpen()){m_dataFile[index]->open(QFile::WriteOnly | QFile::Truncate);}// may have security problem when file is unavaliable as program begins
    fout.setDevice(m_dataFile[index]);
    QString time=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    fout.writeRawData(time.toLocal8Bit(),time.count());
    fout.writeRawData(" ",1);
    if(direction)
    {
        fout.writeRawData("Send:",5);

    }
    else
    {
        fout.writeRawData("Receive:",8);
    }
    fout.writeRawData("\r\n",2);
    fout.writeRawData(data,data.count());
    fout.writeRawData("\r\n",2);
    //fout<<data;
    //fout.flush();
}

void LSaveFile::setBasePath(QString basePath)
{
    m_basePath=basePath;
}

void LSaveFile::changeFile()
{
    int i;
    QString basePath=m_basePath+"/"+QDateTime::currentDateTime().toString("yyyy-MM-dd");
    m_baseDir=new QDir();
    m_baseDir->setPath(basePath);
    QString p= m_baseDir->absolutePath();
    if(!m_baseDir->exists()) { i=m_baseDir->mkpath(m_baseDir->absolutePath());}
    m_itemDir=new QDir*[m_itemCount];
    for(unsigned int i=0;i<m_itemCount;i++)
    {
        m_itemDir[i]=new QDir();
        m_itemDir[i]->setPath(basePath+"/"+m_itemName[i]);
        if(!m_itemDir[i]->exists()) { m_itemDir[i]->mkpath(m_itemDir[i]->absolutePath());};
    }
    QString _fileName=QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss")+".log";
    for(unsigned int i=0;i<m_itemCount;i++)
    {
        if(m_dataFile[i]){if(m_dataFile[i]->isOpen()){ m_dataFile[i]->close();m_dataFile[i]->deleteLater();}}
        m_dataFile[i]=new QFile();
        m_dataFile[i]->setFileName(m_itemDir[i]->absolutePath()+"/"+_fileName);
    }
    //if(m_file1){if(m_file1->isOpen()) { m_file1->close();}}//rename file with close time
    delete m_baseDir;
    delete[] m_itemDir;
}
