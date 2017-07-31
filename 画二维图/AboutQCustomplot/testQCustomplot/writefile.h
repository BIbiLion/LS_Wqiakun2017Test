#ifndef WRITEFILE_H
#define WRITEFILE_H

#include <QObject>
#include <QStringList>
#include <QDataStream>
#include <QFile>
#include <QVector>
#include <QTextStream>
#include <QTimer>
#include <QDateTime>
class QWriteFile : public QObject
{
    Q_OBJECT
public:
    explicit QWriteFile(const QString &savePath,const QStringList & fileNameList ,QObject *parent = 0);
    ~QWriteFile();
    void writeFile(const int & _index,const QString &);

signals:

public slots:
    void changeFileName();
private:
    QStringList m_fileNameList;//文件名
    QVector<QFile*> m_filePtrList;
    QString m_savePath;//保存路径
    QDataStream m_dataStream;
    QTextStream m_textStream;
    QTimer * m_timer;

};

#endif // WRITEFILE_H
