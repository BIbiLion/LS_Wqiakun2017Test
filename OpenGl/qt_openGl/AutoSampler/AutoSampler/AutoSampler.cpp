#include "AutoSampler.h"
#include "ui_AutoSampler.h"

AutoSampler::AutoSampler(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AutoSampler)
{
    ui->setupUi(this);
    this->setGeometry(200,50,800,660);
    glayout =new QGridLayout;
    widget =new QWidget;

    glayout->setContentsMargins(5,5,5,5);
    glayout->setRowStretch(0,1);
    glayout->setRowStretch(1,1);
    glayout->setRowStretch(2,1);
    glayout->setColumnStretch(0,1);
    glayout->setColumnStretch(1,1);
    glayout->setColumnStretch(2,1);

    setCentralWidget(widget);

    glayout->addWidget(ui->scrollArea,0,0,2,2);
    glayout->addWidget(ui->lab_pic,2,0,1,2);
    glayout->addWidget(ui->plainTextEdit,0,2,2,1);
    glayout->addWidget(ui->groupBox,2,2,1,1);
    widget->setLayout(glayout);

    ui->scrollArea->setWidget(&glWidget);
    ui->plainTextEdit->setPlaceholderText("实验指导:");
    ui->lab_pic->setPixmap(QPixmap(":/4.png"));
    ui->groupBox->setDisabled(true);


    connect(&glWidget,SIGNAL(sigInputDisplayLed(uint)),this,SLOT(slotInputDisplayLed(uint)),Qt::QueuedConnection);
    connect(&glWidget,SIGNAL(sigOutputDisplayLed(uint)),this,SLOT(slotOutputDisplayLed(uint)),Qt::QueuedConnection);
    connect(&glWidget,SIGNAL(sigFullScreen()),this,SLOT(slotFullScreen()));
    connect(&glWidget,SIGNAL(sigQuitFullScreen()),this,SLOT(slotQuitFullScreen()));
    ui->statusBar->addPermanentWidget(&glWidget.lab);//addWidget 添加到状态栏的左边   ；addPremantWidget添加到状态栏的右边

}

void AutoSampler::slotFullScreen()
{
      ui->groupBox->setHidden(true);
      ui->lab_pic->setHidden(true);
      ui->plainTextEdit->setHidden(true);
      glayout->addWidget(ui->scrollArea,0,0,3,3);
      this->setWindowFlags(Qt::Window);
      this->showFullScreen();
}

void AutoSampler::slotQuitFullScreen()
{
    glayout->addWidget(ui->scrollArea,0,0,2,2);
    ui->groupBox->setHidden(false);
    ui->lab_pic->setHidden(false);
    ui->plainTextEdit->setHidden(false);
    this->setWindowFlags(Qt::Window);
    this->showNormal();
}

void AutoSampler::slotInputDisplayLed(uint portData)
{
    if( (portData&0x01)==0x00 )//主电机脉冲
    {
        ui->rad_1->setChecked(true);
    }
    else if( (portData&0x01)==0x01 )
    {
        ui->rad_1->setChecked(false);
    }

    if( (portData&0x02)==0x00 )//主电机方向
    {
        ui->rad_0->setChecked(true);
    }
    else if( (portData&0x02)==0x02 )
    {
        ui->rad_0->setChecked(false);
    }

    if( (portData&0x04)==0x00 )//辅电机方向
    {
        ui->rad_2->setChecked(true);
    }
    else if( (portData&0x04)==0x04 )
    {
        ui->rad_2->setChecked(false);
    }

    if( (portData&0x08)==0x00 )//辅电机脉冲
    {
        ui->rad_3->setChecked(true);
    }
    else if( (portData&0x08)==0x08 )
    {
        ui->rad_3->setChecked(false);
    }

}

void AutoSampler::slotOutputDisplayLed(uint portData)
{
    if( (portData&0x08)==0x00 )//初始位置
    {
        qDebug()<<"初始位置被点亮了";
        ui->rad_11->setChecked(true);
    }
    else if( (portData&0x08)==0x08 )
    {
        qDebug()<<"初始位置被熄灭了";
        ui->rad_11->setChecked(false);
    }

    if( (portData&0x10)==0x00 )//up
    {
        ui->rad_12->setChecked(true);
    }
    else if( (portData&0x10)==0x10 )
    {
        ui->rad_12->setChecked(false);
    }

    if( (portData&0x20)==0x00 )//down
    {
        ui->rad_13->setChecked(true);
    }
    else if( (portData&0x20)==0x20 )
    {
        ui->rad_13->setChecked(false);
    }

}

AutoSampler::~AutoSampler()
{
    delete ui;
}
