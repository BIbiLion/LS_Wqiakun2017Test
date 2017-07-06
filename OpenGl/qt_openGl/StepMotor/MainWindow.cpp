#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{ 
    ui->setupUi(this);
    setGeometry(100,100,800,600);
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
    ui->lab_pic->setPixmap(QPixmap(":/3.png"));
    ui->plainTextEdit->setPlaceholderText("实验指导:");
    ui->groupBox->setDisabled(true);

    ui->statusBar->addPermanentWidget(&glWidget.lab);
    connect(&glWidget,SIGNAL(sigDisplayLed6to0(uint)),this,SLOT(slotDisplayLed6to0(uint)));
    connect(&glWidget,SIGNAL(sigDisplayLed7(uint)),this,SLOT(slotDisplayLed7(uint)));
    connect(&glWidget,SIGNAL(sigFullScreen()),this,SLOT(slotFullScreen()));
    connect(&glWidget,SIGNAL(sigQuitFullScreen()),this,SLOT(slotQuitFullScreen()));
}

void MainWindow::slotFullScreen()
{
      ui->groupBox->setHidden(true);
      ui->lab_pic->setHidden(true);
      ui->plainTextEdit->setHidden(true);
      glayout->addWidget(ui->scrollArea,0,0,3,3);
      this->setWindowFlags(Qt::Window);
      this->showFullScreen();
}

void MainWindow::slotQuitFullScreen()
{

    glayout->addWidget(ui->scrollArea,0,0,2,2);
    ui->groupBox->setHidden(false);
    ui->lab_pic->setHidden(false);
    ui->plainTextEdit->setHidden(false);
    this->setWindowFlags(Qt::Window);
    this->showNormal();
}

void MainWindow::slotDisplayLed6to0(uint portData)
{
    //qDebug()<<"portData:::"<<portData<<endl;
    if((portData&0x01)==1)
    {
         ui->rad_0->setChecked(false);
         //qDebug()<<"AAAAAAA"<<endl;
    }

    if((portData&0x01)==0)
    {
        ui->rad_0->setChecked(true);
        //qDebug()<<"BBBBBBBB"<<endl;
    }

     if((portData&0x02)==2)
     {
        ui->rad_1->setChecked(false);
        //qDebug()<<"想前转的指示灯灭了"<<endl;
     }
    if((portData&0x02)==0)
    {
        ui->rad_1->setChecked(true);
        //qDebug()<<"想前转的指示灯亮了"<<endl;
    }
}

void MainWindow::slotDisplayLed7(uint data)
{
    if((data&0x80)==1)
        ui->rad_7->setChecked(false);
    if((data&0x80)==0)
    {
        ui->rad_7->setChecked(true);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
