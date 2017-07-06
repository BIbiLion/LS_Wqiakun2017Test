#include "sttthread.h"

//初始化百度语音的各种参数
sttThread::sttThread(QThread *parent):
    QThread(parent),path("./release/test.wav")//录音文件放的路径
{
    stopFlag=false;

    API_id = "5679929";//百度语音识别账号
    API_key = "gQNLHflZmE3vrEEYxaGrc86z";//百度语音识别密码
    API_secret_key = "8BE67BO6NPaMSTAElqfQp9uUML30L4L7";
    API_access_token="";
    API_language="zh";//语音包录的是中文
    API_record_path="./release/test.wav";
    flag=1;//1:获取API_access_token 0:获取语音结果

    //创建http对象,用于来回发送http请求
   manager = new QNetworkAccessManager(this);
   connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinish(QNetworkReply*)));
   connect(this,SIGNAL(startSTT(QString,QString,QString,QString)),this,SLOT(getText(QString,QString,QString,QString)));
   manager->get(QNetworkRequest(QUrl("https://openapi.baidu.com/oauth/2.0/token?grant_type=client_credentials&client_id=" + API_key + "&client_secret=" + API_secret_key)));

}

//接收到 "开始录音"信号,就执行这个函数->开始录音
void sttThread::startRecord()
{
    file =new QFile();
    file->setFileName(path);//设置其实设置音频文件的存放路径(输入音频名及存放路径)
    bool is_open =file->open(QIODevice::WriteOnly | QIODevice::Truncate);
    if(!is_open)
    {
        qDebug()<<"打开失败失败"<<endl;
        exit(1);
    }

    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/wav");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    input = new QAudioInput(format,this);
    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    QString str=info.deviceName();
    qDebug()<<"使用的录音设备是:"<<str<<endl;
    if(!info.isFormatSupported(format))
    {
        format = info.nearestFormat(format);
    }
   // input = new QAudioInput(format, this);*/

    qDebug()<<"录音开始......"<<endl;
    input->start(file);
}

//接收到 "结束录音"信号,就执行这个函数->结束录音
void sttThread::stopRecord()
{
    input->stop();
    file->close();
    delete file;
    delete input;
     qDebug()<<"录音结束......"<<endl;
     emit startSTT(API_id,API_access_token,API_language,API_record_path);
}

//将录制好的语音发送到百度语音识别服务,让百度解析语音包
void sttThread::getText(QString para_API_id, QString para_API_access_token, QString para_API_language, QString para_API_record_path)
{
    QFile file(para_API_record_path);
    if( !(file.open(QIODevice::ReadWrite)))
    {
        //QMessageBox::warning(this,"警告","打开语音文件失败！");
        qDebug()<<"打开语音文件失败!"<<endl;
        return;
    }

    QDataStream in(&file);
    m_buf =new char[file.size()];
    in.readRawData(m_buf,file.size());
    file.close();

    QString  getTextUrl = "http://vop.baidu.com/server_api?lan=" + para_API_language + "&cuid=" + para_API_id + "&token=" + para_API_access_token;
    QUrl url;
    url.setUrl(getTextUrl);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "audio/wav;rate=8000");//音频的参数,这里写死在程序里了,格式:wav  采样率:8000  单音道

    QByteArray arr = QByteArray(m_buf, file.size());
    manager->post(request,arr);
}

//接收百度语音识别的结果
void sttThread::replyFinish(QNetworkReply *reply)
{
    if(flag==1)
    {
            QString strJsonAccess=reply->readAll();
            qDebug()<<"我是json:"<<strJsonAccess<<endl;
            QScriptValue jsonAccess;
            QScriptEngine engineAccess;
            jsonAccess = engineAccess.evaluate("value = " + strJsonAccess);
            QScriptValueIterator iteratorAccess(jsonAccess);
            while (iteratorAccess.hasNext())
            {
                  iteratorAccess.next();
                  if(iteratorAccess.name()=="access_token")
                      API_access_token = iteratorAccess.value().toString();
             }
            if(API_access_token=="")
            {
                //QMessageBox::warning(0,"警告","access_token口令获取失败！");
                qDebug()<<"获取口令失败"<<endl;
                return;
            }
            flag=0;
            qDebug()<<"获取口令成功"<<endl;
            reply->deleteLater();
    }
    else
    {
            QString strJsonText = reply->readAll();
            qDebug()<<strJsonText<<endl;
            QString strText="";
            QScriptValue jsontext;
            QScriptEngine engineText;
            jsontext = engineText.evaluate("value = " + strJsonText );
            QScriptValueIterator iteratorText(jsontext);
            while (iteratorText.hasNext())
            {
                   iteratorText.next();
                  if(iteratorText.name()=="result")
                  {
                      strText = iteratorText.value().toString();
                      qDebug()<<strText<<endl;

                      emit startAction(strText);
                      break;
                  }
             }
    }
}

//启动线程,如果这个函数结束了,这个线程就退出了
void sttThread::run()
{
    qDebug()<<"线程开始了......"<<endl;
    exec();//类似于一个死循环,一直在这个线程里!
    qDebug()<<"线程退出了......"<<endl;
}

sttThread::~sttThread()
{

}

