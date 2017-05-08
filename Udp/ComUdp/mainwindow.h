#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "./class/serialattri.h"
#include "./Object/serialportobject.h"
#include <QThread>
#include <QUdpSocket>
#include <QFile>
#include <QTextStream>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openComSlot();
    void sendData();
    void DataReV(QByteArray);
    void setSendStatus();
    QString fillFullZero(QString _str,int saveLength,int _beforLength);
private:
    Ui::MainWindow *ui;
    SerialPortObject * m_serialPort1;
    QThread * m_thread1;
    QUdpSocket * m_udpSocket;
    bool m_isSend;
    QFile *m_file;
};

#endif // MAINWINDOW_H
