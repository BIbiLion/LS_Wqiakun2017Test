#include <QCoreApplication>
#include <QDebug>
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
    QString _str("北京");
    qDebug()<<_str;
    return a.exec();
}
