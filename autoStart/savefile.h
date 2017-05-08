#ifndef SAVEFILE_H
#define SAVEFILE_H

#include <QObject>
#include <QTime>
#include <QTimer>
#include <QFile>
#include <QTextStream>
class SaveFile : public QObject
{
    Q_OBJECT
public:
    explicit SaveFile(QString fileParentDir,QObject *parent = 0);
    ~SaveFile();
    void initFile();

    void writeInFile(QStringList dataList);
signals:

public slots:
    void changeFile();
private:
    QTimer * m_timerInterval;//时间间隔定时器
    QFile * m_file;
    QTextStream m_fout;
    QString m_dirPath;

};

#endif // SAVEFILE_H
