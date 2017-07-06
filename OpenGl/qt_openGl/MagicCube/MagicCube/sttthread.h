#ifndef STTTHREAD_H
#define STTTHREAD_H

#include <QWidget>
#include <QMultimedia>
#include <QFile>
#include <QDebug>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioInput>
#include <QTimer>
#include <QAudioOutput>
#include <QFileDialog>
#include <QDialog>
#include <QLabel>
#include <QThread>
#include<QtNetwork>

#include <QJsonDocument>
#include <QJsonParseError>
#include <QFileInfo>
#include<QMessageBox>
#include <QScriptEngine>
#include <QScriptValue>
#include <QScriptValueIterator>
#include <QLabel>

class sttThread:public QThread
{
    Q_OBJECT
public:
    explicit sttThread(QThread *parent = 0);


    ~sttThread();

     QFile *file;
     QString path;
     QAudioInput * input;
     QAudioOutput* output;
     QAudioFormat format;
     volatile bool stopFlag;

     QNetworkAccessManager *manager;
     QNetworkRequest *req;
     QString API_access_token;
     QString API_id;
     QString API_key;
     QString API_secret_key;
     QString API_record_path;
     QString API_language;
     char * m_buf;
     int flag;

signals:
     void startSTT(QString,QString,QString,QString);
     void startAction(QString);

public slots:
    void replyFinish(QNetworkReply * reply);
    void startRecord();
    void stopRecord();
    void getText(QString para_API_id, QString para_API_access_token,QString para_API_language, QString para_API_record_path);


protected:
    void run();

};

#endif // STTTHREAD_H
