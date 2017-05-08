#include "serialportobject.h"
#include <QDebug>

SerialPortObject::SerialPortObject( QObject *parent ):
    QObject(parent),m_serialPort(NULL)
{
    m_serialPort =new QSerialPort(this);
}
SerialPortObject::SerialPortObject(SerialAttri &serialPortAttr,QObject *parent) :
    QObject(parent),m_serialPort(NULL)
{
    m_serialPort =new QSerialPort(this);
    dispalySerialPort();
    initSerialPort(serialPortAttr);
    writeDate("hello");
}

SerialPortObject::~SerialPortObject()
{
    m_serialPort->close();
    m_serialPort->deleteLater();
    m_serialPort=NULL;
}

/*
Descriple:初始化串口
Function:设置 波特率 串口名 etc
author: wuqiankun
time:
*/
void SerialPortObject::initSerialPort(SerialAttri &serialPortAttr)
{

    m_serialPort->setPortName(serialPortAttr.m_portName);
    qDebug()<<m_serialPort->portName();
    if(!m_serialPort->open(QIODevice::ReadWrite))
    {

        qDebug()<<"open serialPort error!!!"+QString("%1%2").arg(m_serialPort->portName()).arg(m_serialPort->errorString());
        return;
    }
    if(!m_serialPort->setBaudRate(serialPortAttr.m_baudRate))
    {
        qDebug()<<QString("can't set baud rate 9600 to port %1,error %2").arg(m_serialPort->portName()).arg(m_serialPort->errorString());
        return;
    }
    if(!m_serialPort->setDataBits(serialPortAttr.m_dateBit))
    {
        qDebug()<<QString("Can't set 8 data bits to port %1, error code %2").arg(m_serialPort->portName()).arg(m_serialPort->errorString());
        return;
    }

    if(!m_serialPort->setParity(serialPortAttr.m_Parity))
    {
        qDebug()<<QString("Can't set no parity to port %1, error code %2").arg(m_serialPort->portName()).arg(m_serialPort->errorString());
        return;
    }
    if(!m_serialPort->setStopBits(serialPortAttr.m_stopBit))
    {

        qDebug()<<QString("Can't set 1 stop bit to port %1, error code %2").arg(m_serialPort->portName()).arg(m_serialPort->errorString());
        return;
    }
    if(!m_serialPort->setFlowControl(serialPortAttr.m_FlowControl))
    {

        qDebug()<<QString("Can't set no flow control to port %1, error code %2").arg(m_serialPort->portName()).arg(m_serialPort->errorString());
        return;
    }

     m_serialPort->clear();
     m_serialPort->clearError();

    connect(m_serialPort,SIGNAL(readyRead()),this,SLOT(reveiveData()));
    connect(m_serialPort,SIGNAL(error(QSerialPort::SerialPortError)),this,SLOT(errorDispaly(QSerialPort::SerialPortError)));
    // connect(m_serialPort,SIGNAL(objectNameChanged(QString)),this,SLOT());
    // connect(m_serialPort,SIGNAL(baudRateChanged(qint32,QSerialPort::Directions)),this,SLOT(BanudRateChangeSlot(qint32,QSerialPort::Directions)));
    // connect(m_serialPort,SIGNAL(dataBitsChanged(QSerialPort::DataBits)),this,SLOT(dataBitsChangedSlot(QSerialPort::DataBits)));
    // connect(m_serialPort,SIGNAL(parityChanged(QSerialPort::Parity)),this,SLOT(parityChangedSlot(QSerialPort::Parity)));
    // connect(m_serialPort,SIGNAL(stopBitsChanged(QSerialPort::StopBits)),this,SLOT(stopBitsChangedSlot(QSerialPort::StopBits)));
    // connect(m_serialPort,SIGNAL(flowControlChanged(QSerialPort::FlowControl)),this,SLOT(flowControlChangedSlot(QSerialPort::FlowControl)));
}

void SerialPortObject::changePortName(const QString & portName)
{
    SerialAttri _seriaAttri(m_serialPort->portName(),m_serialPort->baudRate(),m_serialPort->parity(),\
                            m_serialPort->dataBits(),m_serialPort->stopBits());
    m_serialPort->disconnect();
    m_serialPort->close();

    _seriaAttri.setPortName(portName);
    initSerialPort(_seriaAttri);
}

/*
Descriple:修改界面的波特率，更改serialPort的波特率
Function:
author: wuqiankun
time:
*/
void SerialPortObject::changeBrauRate(const QString & braudRate)
{
    qint32 _baudRate= (qint32)braudRate.toInt();
    BanudRateChangeSlot(_baudRate);
    //BanudRateChangeSlot(_baudRate);
}

void SerialPortObject::changeDataBits(const QString &dataBita)
{
    QSerialPort::DataBits _dataBits;
    switch (dataBita.toInt())
    {
    case 5:
        _dataBits=QSerialPort::Data5;
        break;
    case 6:
        _dataBits=QSerialPort::Data6;
        break;
    case 7:
        _dataBits=QSerialPort::Data7;
        break;
    case 8:
        _dataBits=QSerialPort::Data8;
        break;

    default://-1
        _dataBits=QSerialPort::UnknownDataBits;
        break;
    }
    dataBitsChangedSlot(_dataBits);
}

void SerialPortObject::changeParity(const QString & parity)
{
    QSerialPort::Parity  _parity;
    if(parity.contains("无校验"))
    {
        _parity=QSerialPort::NoParity;
    }
    else if(parity.contains("奇校验"))
    {
        _parity=QSerialPort::EvenParity;
    }
    else if(parity.contains("偶校验"))
    {
        _parity=QSerialPort::OddParity;
    }
    parityChangedSlot(_parity);
}

void SerialPortObject::changeStopBits(const QString &stopBits)
{
    QSerialPort::StopBits _stopBit;
    switch (stopBits.toInt())
    {
    case 1:
        _stopBit=QSerialPort::OneStop;
        break;
    case 2:
        _stopBit=QSerialPort::TwoStop;
        break;
    default:
        break;
    }
    stopBitsChangedSlot(_stopBit);
}






void SerialPortObject::BanudRateChangeSlot(qint32 baudRate, QSerialPort::Directions directions)
{
    //    if(m_serialPort->isOpen())
    //    {
    //        m_serialPort->close();
    //    }

    //    SerialAttri _serialAttrial(m_serialPortAttr);
    //    _serialAttrial.m_baudRate=baudRate;
    //    initSerialPort(_serialAttrial);

    if(!m_serialPort->setBaudRate(baudRate))
    {
        qDebug()<<QString("can't set baud rate 9600 to port %1,error %2").arg(m_serialPort->portName()).arg(m_serialPort->errorString());
        return;
    }

}

void SerialPortObject::dataBitsChangedSlot(QSerialPort::DataBits dataBits)
{
    if(!m_serialPort->setDataBits(dataBits))
    {
        qDebug()<<QString("Can't set 8 data bits to port %1, error code %2").arg(m_serialPort->portName()).arg(m_serialPort->errorString());
        return;
    }

}

void SerialPortObject::parityChangedSlot(QSerialPort::Parity parity)
{
    if(!m_serialPort->setParity(parity))
    {
        qDebug()<<QString("Can't set no parity to port %1, error code %2").arg(m_serialPort->portName()).arg(m_serialPort->errorString());
        return;
    }

}

void SerialPortObject::stopBitsChangedSlot(QSerialPort::StopBits stopBits)
{
    if(!m_serialPort->setStopBits(stopBits))
    {

        qDebug()<<QString("Can't set 1 stop bit to port %1, error code %2").arg(m_serialPort->portName()).arg(m_serialPort->errorString());
        return;
    }
}

void SerialPortObject::flowControlChangedSlot(QSerialPort::FlowControl flow)
{
    if(!m_serialPort->setFlowControl(flow))
    {
        qDebug()<<QString("Can't set no flow control to port %1, error code %2").arg(m_serialPort->portName()).arg(m_serialPort->errorString());
        return;
    }
}

/*
Descriple:接受从端口，进行的数据
Function:
author: wuqiankun
time:
*/
void SerialPortObject::reveiveData()
{
    QByteArray _by=  m_serialPort->readAll();
//    qDebug()<<"by----"<<(char)_by[0]    ;
//    qDebug()<<"-----------------------------";
//    char _c=(char)_by[0];
//    QByteArray _outPut;
//    qDebug()<<_c<<"..............."<<_outPut.fill(_c,1).toHex();
//    //将串口数据放到相应的数据缓存区
//    //得到数据的长度   长度 ==sizeof(串口收据大小)  crc校验   校验数据通过后根据状态进行相应的显示

//    qDebug()<<"\n\n\n---------------";
//    int i=0;
//    while(i< _by.count())
//    {
//        unsigned char _value=(unsigned char)_by.at(i);
//        qDebug()<<_value;
//        if(_value == 0xe0)
//        {
//            qDebug()<<"equal";
//        }
//        ++i;
//    }
    qDebug()<<"com ReCEIVED:"<<_by;
    sendComRev(_by);

}

void SerialPortObject::writeDate(QString  writeDate)
{
    bool _flg= m_serialPort->isWritable();
    qDebug()<<"this is can be Write: "<<_flg;
    char date[6];
    date[0]=0xE0;
    date[1]=0x06;
    date[2]=0x07;
    date[3]=0x00;
    date[4]=0x07;
    date[5]=0xB3;
    m_serialPort->write(date,6);
}


void SerialPortObject::errorDispaly(QSerialPort::SerialPortError _statError)
{
    if(_statError != QSerialPort::NoError)
    {
        qDebug()<<"SerialPort error infomations: "<<m_serialPort->errorString();
    }
}


/*
Descriple: 枚举可以使用串口
Function:
author: wuqiankun
time:
*/
void SerialPortObject::dispalySerialPort()
{
    QList<QSerialPortInfo> _listSerialPort= QSerialPortInfo::availablePorts();
    foreach (QSerialPortInfo _protInfo, _listSerialPort)
    {
        if(!_protInfo.isNull())
        {
            qDebug()<<_protInfo.portName();
        }
    }
}
