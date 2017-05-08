#ifndef LSAVEFILE_H
#define LSAVEFILE_H

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QString>
#include <QTimer>
#include <QDateTime>
#include <QObject>
#include <QDataStream>
#include <QTextStream>


class LSaveFile:public QObject
{
    Q_OBJECT
public:
    //LSaveFile();
    LSaveFile(QString basePath, unsigned int itemCount, QStringList& itemNames, unsigned int fileTimerVar);
    ~LSaveFile();

    QFile* getDataFile(unsigned int i);
    void writeFile(unsigned int index,QStringList data);
    void writeFileBinary(unsigned int index, QByteArray data, bool direction=0);

    void setBasePath(QString basePath);
private slots:
    void changeFile();

private:
    QString m_basePath;
    unsigned int m_itemCount;
    QStringList m_itemName;
    QDir *m_baseDir;
    QDir **m_itemDir;
    QFile **m_dataFile;
    unsigned int m_fileTimerVar;
    QTimer *m_fileTimer;

    QDataStream fout;

    //int (*writeFile)();
    //QTimer *m_writeTimer;
};

#endif // LSAVEFILE_H
