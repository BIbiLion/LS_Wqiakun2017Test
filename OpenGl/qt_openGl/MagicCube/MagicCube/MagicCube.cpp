#include "MagicCube.h"
#include "ui_MagicCube.h"
#include <QApplication>

//windows窗口布局
MagicCube::MagicCube(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MagicCube)
{
    ui->setupUi(this);
    this->setGeometry(100,100,800,600);
    ui->scrollArea->setWidget(&magicCube);
    ui->Slider_bigSmall->setMaximum(360);
    ui->Slider_leftRight->setMaximum(360);
    ui->Slider_beforAfter->setMaximum(360);
    connect(ui->Slider_bigSmall,SIGNAL(valueChanged(int)),&magicCube,SLOT(changeX(int)));
    connect(ui->Slider_leftRight,SIGNAL(valueChanged(int)),&magicCube,SLOT(changeY(int)));
    connect(ui->Slider_beforAfter,SIGNAL(valueChanged(int)),&magicCube,SLOT(changeZ(int)));
    connect(this,SIGNAL(startRecord()),&stt,SLOT(startRecord()));
    connect(this,SIGNAL(stopRecord()),&stt,SLOT(stopRecord()));
    connect(&stt,SIGNAL(startAction(QString)),&magicCube,SLOT(Action(QString)));
}

//键盘事件,v:发送开始录音信号,s:结束录音信号
void MagicCube::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_V)
    {
        stt.start();
        emit startRecord();
    }

    if(e->key()==Qt::Key_S)
        emit stopRecord();
}

//程序结束,清除类所占的内存空间
MagicCube::~MagicCube()
{
    delete ui;
}

//main函数,创建Windows窗口,显示窗口
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MagicCube w;
    w.show();

    return a.exec();
}
