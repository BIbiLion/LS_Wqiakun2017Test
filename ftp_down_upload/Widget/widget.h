#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "thread/threadftp.h"
#include <QThread>
#include <QUdpSocket>


namespace Ui {
class widget;
}

class widget : public QWidget
{
    Q_OBJECT

public:
    explicit widget(QWidget *parent = 0);
    ~widget();


    QStringList getLocalFileList(QString dirStr);
public slots:
    void progressValueSlot(qint64,qint64);
    void uploadFinshed();
    void downFinshed();

    void listinfoListSlot(QList<QUrlInfo>);
    void downProgressValueSlot(qint64 _value, qint64 _total);
    void downListinfoListSlot(QList<QUrlInfo> _urlInfoList);

    void downFileProgressValue(int curFileCount,int totalFileCount);
    void uploadFileProgressValue(int curFileCount,int totalFileCount);
protected:
    void initUdpSocket();

    void downIpLineEdit();
    QString getDownLoadIpStr()const;

    void uploadIpLineEdit();
    QString getUpLoadIpStr()const;

    void downLoadFtpInit();
    void uploadFtpInit();
protected slots:
    void processPendingDatagrams();
private:
    Ui::widget *ui;

    ObjectFtp * m_downFtp;
    QList<QUrlInfo> m_downServerUrlInfoList;//下载服务器中的 某文件的文件
    QString m_downServerDownDir;//下载服务器的 下载目录，通过UDP获得


    ObjectFtp * m_workFtp;
    QList<QUrlInfo> m_remoteUrlInfoList;

    QUdpSocket *m_udpSocket;//udpSocket结束 服务器发来的下载文件路径

};

#endif // WIDGET_H
