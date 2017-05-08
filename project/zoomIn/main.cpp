#include "mainform.h"
#include <QApplication>
#include <QDir>

#include <QPixmap>
#include <QScreen>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //设置程序路径为相对路径
    QDir::setCurrent(QApplication::applicationDirPath());
    MainForm w;
    w.show();

    //test
  QPixmap _fullPixmap=  QApplication::primaryScreen()->grabWindow(0);
  QPixmap  _tempPix = _fullPixmap.copy(QRect(0, 0, 50, 50))
            .scaled(1800, 1800, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
 _tempPix.save("wqiankunTest.png");
    return a.exec();
}
