#include "serialattri.h"

SerialAttri::SerialAttri(QString  portName,QString  bandRate,QString  parity,QString dateBits,QString  stopBit)
    :m_portName(portName),m_baudRate((qint32)(bandRate.toInt())),m_FlowControl(QSerialPort::NoFlowControl)
{

    setParity(parity);
    setDateBits(dateBits);
    setStopBit(stopBit);
}


SerialAttri::SerialAttri(QString  portName,qint32  baudRate,QSerialPort::Parity  Parity,QSerialPort::DataBits dateBits,QSerialPort::StopBits  stopBit)
{
    m_portName=portName;
    m_baudRate =baudRate;
    m_Parity =Parity ;
    m_dateBit=dateBits;
    m_stopBit=stopBit;
    m_FlowControl=QSerialPort::NoFlowControl;
}
SerialAttri::~SerialAttri()
{
}


SerialAttri::SerialAttri(SerialAttri& serialAttri)
{
    m_portName=serialAttri.m_portName;
    m_baudRate =serialAttri.m_baudRate;
    m_Parity =serialAttri.m_Parity ;
    m_dateBit=serialAttri.m_dateBit;
    m_stopBit=serialAttri.m_stopBit;
    m_FlowControl=serialAttri.m_FlowControl;

}

void SerialAttri::setPortName(const QString& portName)
{

    m_portName=portName;
}

void SerialAttri::setBandRate(const QString & bandRate)
{
    m_baudRate=(qint32)bandRate.toInt();
}

void SerialAttri::setParity(QSerialPort::Parity &parity)
{
    m_Parity=parity;
}

void SerialAttri::setParity(QString &parity)
{
    if(parity.contains("无校验"))
    {
        m_Parity=QSerialPort::NoParity;
    }
    else if(parity.contains("奇校验"))
    {
        m_Parity=QSerialPort::EvenParity;
    }
    else if(parity.contains("偶校验"))
    {
        m_Parity=QSerialPort::OddParity;
    }
}

void SerialAttri::setDateBits(QSerialPort::DataBits & _dataBits)
{
    m_dateBit=_dataBits;
}

void SerialAttri::setDateBits(QString & dateBit)
{
    switch (dateBit.toInt())
    {
    case 5:
        m_dateBit=QSerialPort::Data5;
        break;
    case 6:
        m_dateBit=QSerialPort::Data6;
        break;
    case 7:
        m_dateBit=QSerialPort::Data7;
        break;
    case 8:
        m_dateBit=QSerialPort::Data8;
        break;

    default://-1
        m_dateBit=QSerialPort::UnknownDataBits;
        break;
    }
}

void SerialAttri::setStopBit(QString &stopBit)
{
    switch (stopBit.toInt())
    {
    case 1:
        m_stopBit=QSerialPort::OneStop;
        break;
    case 2:
        m_stopBit=QSerialPort::TwoStop;
        break;
    default:
        break;
    }
}

void SerialAttri::setStopBit(QSerialPort::StopBits &stopBit)
{
    m_stopBit=stopBit;
}
