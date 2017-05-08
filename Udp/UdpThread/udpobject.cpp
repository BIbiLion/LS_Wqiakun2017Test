#include "udpobject.h"

UdpObject::UdpObject(QString mes,QObject *parent) :
    QObject(parent)
{
}

UdpObject::~UdpObject()
{
}

void UdpObject::startThread()
{

    QUdpSocket* udpSocket = new QUdpSocket(this);


    int messageNo = 1;
    QByteArray datagram = "Broadcast message " + QByteArray::number(messageNo)+QByteArray("nice LSGK");
    qint64 _sendedDate= udpSocket->writeDatagram(datagram.data(), datagram.size(),
                                                 QHostAddress::Broadcast, 45454);
    emit end();
}


