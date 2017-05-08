#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCursor>
#include <QMouseEvent>

#include "simpleitem.h"
#include "mygraphicsview.h"
#include "myscene.h"

#include <QLibrary>
#include <QDebug>

//没有纯虚函数的引用
//#include "./include/plugin/c_shareddll.h"
#include "./include/test/ftp.h"

//base
#include "./include/baseClassPlugin/c_shareddll.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*
  m_graphicsScene= new QGraphicsScene(00, 00, 400, 400);


  m_graphicsView= new QGraphicsView(m_graphicsScene,this);
  m_graphicsView->setAcceptDrops(true);
  m_graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

  ui->middleLayout->addWidget(m_graphicsView);
 // m_graphicsScene->addText("Hello world!");


 // m_graphicsScene->addItem();
  SimpleItem * _item = new SimpleItem(m_graphicsView);
  m_graphicsScene->addItem(_item);



  //right
   MyScene * _myScene= new MyScene();
  m_view= new MyGraphicsView(_myScene);
  SimpleItem * _item2 = new SimpleItem(m_view);
  m_graphicsScene->addItem(_item2);
 _myScene->addItem(_item);
 _myScene->addLine(0,0,100,100);
  ui->rightLayout->addWidget(m_view);


  m_toolButton = new ToolButton(this);
  ui->leftLayout->addWidget(m_toolButton);
  m_toolButton->setText("Circle");
  m_toolButton->setIcon(QIcon(":/image/Lighthouse.jpg"));
  m_toolButton->setIconSize(QSize(80,80));
  m_toolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
 // m_toolButton->setAcceptDrops(true);
*/
    QString _stra=name();
    Ftp ftp;
    qDebug()<<_stra<<ftp.testMemF();


    //base pure
    QString _str("E:\\LSwuqiankun\\LSWuqiankun\\freamWork_configuration\\C_sharedDllBase.dll");

    QLibrary myLib(_str);
    qDebug()<<"是不是library:"<< myLib.isLibrary(_str);
    qDebug()<<"是不是加载上："<< myLib.load();
    //导出类 和成员函数
    typedef  C_sharedDll*(*ClassdLL)();  //因为类型还需要加头文件
    ClassdLL _classFunctionPtr = (ClassdLL) myLib.resolve("getTestDll");
    if(_classFunctionPtr)
    {
        C_sharedDll* _classPtr =_classFunctionPtr();
        qDebug()<<_classPtr->hhh();
    }




    /*
    //load plugin dll
    QString _str("E:\\LSwuqiankun\\LSWuqiankun\\freamWork_configuration\\debug\\C_sharedDll.dll");

    QLibrary myLib(_str);
    qDebug()<<"是不是library:"<< myLib.isLibrary(_str);
    qDebug()<<"是不是加载上："<< myLib.load();
    //函数指针

    typedef QString (*FunctionPtr)();

    //全局函数
    FunctionPtr _functionPtr = (FunctionPtr) myLib.resolve("getName");
    if(!_functionPtr )
    {
        qDebug()<<"解析失败";
        return;
    }
    QString _name= _functionPtr();
    qDebug()<<"000000000000000000000"<<_name;

    //导出类 和成员函数
      typedef  C_sharedDll*(*ClassdLL)();  //因为类型还需要加头文件
        ClassdLL _classFunctionPtr = (ClassdLL) myLib.resolve("getTestDll");
        if(_classFunctionPtr)
        {
          C_sharedDll* _classPtr =_classFunctionPtr();
         QLabel * _lable= (QLabel *) _classPtr->createWidget();
         qDebug()<<_lable->text();

          ui->leftLayout->addWidget(_classPtr->createWidget());
        }
*/

}

MainWindow::~MainWindow()
{
    delete ui;
}





