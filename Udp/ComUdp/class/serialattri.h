/*串口属性类： 串口号  波特率  等信息
*/
#ifndef SERIALATTRI_H
#define SERIALATTRI_H

#include <QSerialPort>
class SerialAttri
{
public:

    SerialAttri(QString  portName,QString  bandRate="9600",QString  Parity="无校验",QString dateBits="8",QString  stopBit="1");
     SerialAttri(QString  portName,qint32  bandRate,QSerialPort::Parity  Parity,QSerialPort::DataBits dateBits,QSerialPort::StopBits  stopBit);
    SerialAttri(SerialAttri&);
    ~SerialAttri();

public:
    void setPortName(const QString& portName);
    void setBandRate(const QString & bandRate);

    void setParity(QString &parity);
    void setParity(QSerialPort::Parity &parity);

    void setDateBits(QString & dateBit);
    void setDateBits(QSerialPort::DataBits & _dataBits);

    void setStopBit(QString &stopBit);
    void setStopBit(QSerialPort::StopBits &stopBit);

public://应该是private和getArrtial()
    QString m_portName;
    qint32 m_baudRate;
    QSerialPort::DataBits m_dateBit;
    QSerialPort::Parity   m_Parity;
    QSerialPort::StopBits m_stopBit;
    QSerialPort::FlowControl m_FlowControl;

};

#endif // SERIALATTRI_H
