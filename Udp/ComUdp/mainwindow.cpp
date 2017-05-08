#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#define LngLength  (11)
#define LatLength  (10)
#define HeightLength (7)
#define AngleLength (7)
#define SpeedLength (5)
#define zeroChar    ('0')
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),m_isSend(false),m_file(NULL),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->openBtn,SIGNAL(clicked()),this,SLOT(openComSlot()));

    m_thread1=new QThread;
    m_udpSocket=new QUdpSocket(this);
    QList<QSerialPortInfo> _listSerialPort= QSerialPortInfo::availablePorts();
    QList<QString> _listName;
    foreach (QSerialPortInfo _portInfo, _listSerialPort)
    {
        if(_portInfo.isValid())
        {
            _listName.push_back(_portInfo.portName());
        }
    }
    ui->comboBox_comPort->addItems(_listName);
    ui->lineEdit_UdpPort->setText("7001");

    m_serialPort1= new SerialPortObject();
    connect(ui->pauseBtn,SIGNAL(clicked()),this,SLOT(setSendStatus()));
    //connect(ui->openBtn,SIGNAL(clicked()),this,SLOT();

    //QCoreApplication::applicationDirPath()+"/data.ini";
    QString _fileName=QDateTime::currentDateTime().toString("yyyy-mm-dd-hh-mm-ss")+".txt";;
    m_file=new QFile(QCoreApplication::applicationDirPath()+"/"+_fileName);

    connect(ui->closeBtn,SIGNAL(clicked()),this,SLOT(close()));
}

MainWindow::~MainWindow()
{
    if(m_file !=NULL && m_file->isOpen())
    {
        m_file->close();
        m_file->deleteLater();
        m_file=NULL;
    }
    m_serialPort1->deleteLater();
    m_serialPort1=NULL;

    delete ui;
}
void MainWindow::setSendStatus()
{
    if(m_isSend)
    {
        ui->pauseBtn->setText("udp发送数据");
    }
    else
    {
        ui->pauseBtn->setText("UDP暂停发送数据");
    }
    m_isSend=!m_isSend;
}

void MainWindow::openComSlot()
{
    SerialAttri _seriaAttri(ui->comboBox_comPort->currentText());

    m_serialPort1->initSerialPort(_seriaAttri);
    connect(m_serialPort1,SIGNAL(sendComRev(QByteArray)),this,SLOT(DataReV(QByteArray)));
    m_serialPort1->moveToThread(m_thread1);
    m_thread1->start();
}

static QString angleConver(QString & _angleStr)
{
    QString _result;
    int _commaPos=_angleStr.indexOf('.');
    QString _houzhuiStr=_angleStr.mid(_commaPos+1);

    QString _frontPos=_angleStr.left(_commaPos);

    QString _fen=_frontPos.right(2);
    QString _dushu = _frontPos.left(_frontPos.count()-2);
    //qDebug()<<"------------"<<_houzhuiStr<<_frontPos<< _fen<<_dushu;

    bool _isok;
    float _resultF= (_dushu.toFloat(&_isok) )+(_fen.toFloat(&_isok))/60.0+(_houzhuiStr.toFloat(&_isok) )/600000.0;

    QString _ss;
    _result.append( _ss.setNum(_resultF,'g',6) );

    return _result;
}

struct Packg
{
    QString m_dataTime;
    QString m_lng;
    QString m_lat;
    QString m_height;
    QString m_posAngle;
    QString m_speed;
};

void  MainWindow::DataReV(QByteArray _by)
{
    //解析COM数据
    qDebug()<<"Com Received"<<_by;
    QString _dataRev(_by);
    // qDebug()<<"--------RecviedData:"<<_revStr;

    QStringList _dataList= _dataRev.split("\n");
    Packg _pack;
    QString _pdataStr,_pTimeStr,_lngStr,_latStr,_height,_speedStr,_posAngle,_eWStr,_nSStr;
    foreach (QString _revStr, _dataList)
    {
        if(_revStr.startsWith("$GPGGA")  )
        {

            QList<QString> _dataList=_revStr.split(',');

            _pTimeStr=_dataList[1];
            _pTimeStr=_pTimeStr.left(_pTimeStr.indexOf('.'));

            _latStr=_dataList[2];
            _latStr= angleConver(_latStr);
            {
                float _latF=_latStr.toFloat();
                QString _ss;
                _latStr=_ss.setNum(_latF,'f',6);
            }

            QString _nSStr=_dataList[3];


            _lngStr=_dataList[4];
            _lngStr=angleConver(_lngStr);

            {
                float _lngf=_lngStr.toFloat();
                QString _ss;
                _lngStr=_ss.setNum(_lngf,'f',6);
            }
            _eWStr=_dataList[5];

            _height=_dataList[11];
            if(_height.isEmpty())
            {
                _height="+0000.0";
            }
            else
            {
                float _heightf=_height.toFloat();
                QString _ss;
                _height=_ss.setNum(_heightf,'f',1);
            }

            if(_nSStr.contains("N"))
            {
                _latStr.prepend("+");
            }
            else
            {
                _latStr.prepend("-");
            }

            if(_eWStr.contains("E"))
            {
                _lngStr.prepend("+");
            }
            else
            {
                _lngStr.prepend("-");
            }
        }
        if(_revStr.startsWith("$GPRMC"))
        {
            QList<QString> _dataList=_revStr.split(',');
             _speedStr=_dataList[7];
            if(_speedStr.isEmpty())
            {
                _speedStr="000.0";
            }
            else
            {
                float _speedF=_speedStr.toFloat()*1.852;
                QString _ss;
                _speedStr=_ss.setNum(_speedF,'f',1);
            }

             _posAngle=_dataList[8];
            if(_posAngle.isEmpty() || _posAngle.isNull())
            {
                _posAngle="000.000";
            }
            else
            {
                float _posAngleF=_posAngle.toFloat();
                QString _ss;
                _posAngle=_ss.setNum(_posAngleF,'f',3);
            }

            _pdataStr=_dataList[9];
        }
    }

    //    _lngStr = _lngStr.leftJustified(LngLength, zeroChar);
    //    _latStr = _latStr.leftJustified(LatLength, zeroChar);
    //    _height = _height.leftJustified(HeightLength, zeroChar);
    //    _speedStr = _speedStr.leftJustified(SpeedLength, zeroChar);
    //    _posAngle = _posAngle.leftJustified(AngleLength, zeroChar);


    _lngStr= fillFullZero(_lngStr,LngLength,4);
    _latStr = fillFullZero(_latStr,LatLength,3 );
    _height = fillFullZero(_height,HeightLength, 5);
    _speedStr = fillFullZero(_speedStr,SpeedLength, 3);
    _posAngle = fillFullZero(_posAngle,AngleLength, 3);


    _pack.m_height=_height;
    _pack.m_lat=_latStr;
    _pack.m_lng=_lngStr;
    _pack.m_posAngle=_posAngle;
    _pack.m_speed=_speedStr;
    _pack.m_dataTime="20";

    _pack.m_dataTime+=_pdataStr+_pTimeStr;

    QString _send=_pack.m_dataTime+","+_pack.m_lng+","+_pack.m_lat+","+_pack.m_height+","+_pack.m_posAngle+","+_pack.m_speed;//_pack.m_posAngle+","+_pack.m_speed;

    {
        //qDebug()<<_pack.m_lng<<_pack.m_lat;
        qDebug()<<"jjjjj";
        m_udpSocket->writeDatagram( _send.toStdString().c_str(),_send.count(),
                                    QHostAddress::LocalHost,ui->lineEdit_UdpPort->text().toInt() );//
    }
    qDebug()<<"----------"<<_send;
    if (m_file->isOpen()==false )
    {
        if(m_file->open(QFile::WriteOnly | QFile::Truncate))
        {
            QTextStream out(m_file);
            out<<_send.replace(",","\t")+"\n";
        }
    }
    else
    {
        QTextStream out(m_file);
        out<<_send.replace(",","\t")+"\n";
    }


   // if(m_isSend)


}



void MainWindow::sendData()
{

}

/*
Descriple:
Function:   int saveLength 保存的总长度， _beforeLength 小数点前保留的位数,
            无符号位
author: wuqiankun
time:
*/
QString MainWindow::fillFullZero(QString _str,int saveLength,int _beforSaveLength)
{
    QString _result(_str);
    if(_str.startsWith("+") || _str.startsWith("-"))// 以+ -以为
    {
        int _commPos=_result.indexOf('.');
        int _insertLength=_beforSaveLength-_commPos;

        for(int i=0;i<_insertLength;++i)
        {
            _result.insert(1,'0');
        }
        int _aftherLength=saveLength-_beforSaveLength-1;
        _insertLength=saveLength-_result.count();
       // int _curCommPos=_result.indexOf(".")

        for(int i=0; i<_insertLength;++i)
        {
            _result.append("0");
        }
    }
    else
    {
        int _commPos=_result.indexOf('.');
        int _insertLength=_beforSaveLength-_commPos;

        for(int i=0;i<_insertLength;++i)
        {
            _result.insert(0,'0');
        }
//        int _aftherLength=saveLength-_beforSaveLength-1;
//        int _curCommPos=_result.indexOf(".");
         _insertLength=saveLength-_result.count();

        for(int i=0; i<_insertLength;++i)
        {
            _result.append("0");
        }
    }


    //    int _commaPos=_str.indexOf('.');
    //    if(_commaPos== -1)//没有找到
    //    {
    //    }
    //    int _fillBerforZero=_beforSaveLength- _commaPos-1;
    //    for(int i=0;i<_fillBerforZero;++i)
    //    {
    //        _result.prepend("0");
    //    }

    //    int _aftherSaveLength=saveLength-_beforSaveLength-1;
    // //error
    //  if(_aftherSaveLength<=0)
    //  {
    //       return _result;
    //  }
    //  else
    //  {
    //      for(int i =0;  i<_aftherSaveLength ;++i )
    //      {
    //          _result.append("0");
    //      }
     return _result;
    //  }


}

