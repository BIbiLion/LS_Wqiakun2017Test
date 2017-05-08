#ifndef UDPOBJECT_H
#define UDPOBJECT_H

#include <QObject>
#include <QUdpSocket>
class UdpObject : public QObject
{
    Q_OBJECT
public:
    explicit UdpObject(QString mes=QString(),QObject *parent = 0);
    ~UdpObject();

signals:
    void end();
public slots:
    void startThread();

private:
    QUdpSocket *m_udpSocket;
    
};

#endif // UDPOBJECT_H
