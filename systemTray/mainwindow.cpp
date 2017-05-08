#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_systemTrayIcon= new  QSystemTrayIcon(this);
    m_systemTrayIcon->setIcon(QIcon(":/image/light"));
    connect(m_systemTrayIcon , SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(systemTrayIconActived(QSystemTrayIcon::ActivationReason)));

    m_systemTrayIcon->setToolTip("信息化单元");
    m_systemTrayIcon->show();

    m_quitAction = new QAction(m_systemTrayIcon);
    m_quitAction->setIconText("退出");
    QObject::connect(m_quitAction, SIGNAL(triggered(bool)), this, SLOT(close()));


   m_menu= new QMenu();
   m_menu->addAction(m_quitAction);
   m_systemTrayIcon->setContextMenu(m_menu);
}





MainWindow::~MainWindow()
{
    if(m_menu !=NULL)
    {
        m_menu->deleteLater();
        m_menu=NULL;

    }

    if(m_systemTrayIcon !=NULL)
    {
        m_systemTrayIcon->deleteLater();
        m_systemTrayIcon=NULL;
    }

    delete ui;
}

void MainWindow::systemTrayIconActived(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    //点击托盘显示窗口
    case QSystemTrayIcon::Trigger:
    {
        this->show();
        break;
    }
        //双击托盘显示窗口
    case QSystemTrayIcon::DoubleClick:
    {
        this->hide();
        break;
    }
    default:
        break;
    }
}
