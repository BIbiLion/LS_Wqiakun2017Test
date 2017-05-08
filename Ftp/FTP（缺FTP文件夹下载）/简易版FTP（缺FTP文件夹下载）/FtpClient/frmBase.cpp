
#include <Windows.h>
#include <QDesktopWidget>
#include <QBitmap>
#include "frmBase.h"

/***************************************************************************
* 处理内容： 构造函数
* @param     parent		父容器
* @return    无
* @exception 
****************************************************************************/
FBase::FBase(QWidget *parent) : QWidget(parent),m_bCanResize(true),formResult(UnKnown),
dialogEventLoop(NULL)
{
	baseUI = new Ui::frmBase;
	baseUI->setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);
	createView();
	baseUI->labTitle->installEventFilter(this);
	
	connect(baseUI->btnMin, SIGNAL(clicked()), this, SLOT(btnMinClicked()));
	connect(baseUI->btnMax, SIGNAL(clicked()), this, SLOT(btnMaxClicked()));
	connect(baseUI->btnClose, SIGNAL(clicked()), this, SLOT(btnCloseClicked()));
}

/***************************************************************************
* 处理内容： 析构函数
* @param     parent		父容器
* @return    无
* @exception 
****************************************************************************/
FBase::~FBase()
{
	delete baseUI;
}

/***************************************************************************
* 处理内容： 设置是否可改变窗体大小，即最大化、最小化按钮是否可见
* @param     isCanResize		是否可以改变大小
* @return    无
* @exception 
****************************************************************************/
void FBase::setResize(bool isCanResize)
{
	m_bCanResize = isCanResize;
	if(isCanResize)
	{
		baseUI->btnMin->setVisible(true);
		baseUI->btnMax->setVisible(true);
	}
	else
	{
		baseUI->btnMin->setVisible(false);
		baseUI->btnMax->setVisible(false);
	}
}
/***************************************************************************
* 处理内容： 初始化界面
* @param     
* @return    无
* @exception 
****************************************************************************/
void FBase::createView()
{
	baseUI->widTopRight->setAutoFillBackground(true);
	QPixmap pixmapTemp(":/img/HeaderRightSide.png");
	QPalette palTemp(baseUI->widTopRight->palette());
	palTemp.setBrush(QPalette::Window, QBrush(pixmapTemp));
	baseUI->widTopRight->setPalette(palTemp);
	baseUI->widTopRight->setMask(pixmapTemp.mask());

	baseUI->widTitle->setStyleSheet("background-image:url(:img/HeaderBackground.png);");
	baseUI->widTopLeft->setAutoFillBackground(true);
	QPixmap pixmapTemp3(":/img/HeaderLeftSide.png");
	QPalette palTemp3(baseUI->widTopLeft->palette());
	palTemp3.setBrush(QPalette::Window, QBrush(pixmapTemp3));
	baseUI->widTopLeft->setPalette(palTemp3);
	baseUI->widTopLeft->setMask(pixmapTemp3.mask());

	baseUI->widLeft->setStyleSheet("background-image:url(:img/LeftBorder.png);");
	baseUI->widRight->setStyleSheet("background-image:url(:img/RightBorder.png);");
	
	//baseUI->widButtomLeft->setStyleSheet("background-image:url(:img/FooterLeftSide.png);");
	baseUI->widButtomLeft->setAutoFillBackground(true);
	QPixmap pixmapTemp1(":/img/FooterLeftSide.png");
	QPalette palTemp1(baseUI->widButtomLeft->palette());
	palTemp1.setBrush(QPalette::Window, QBrush(pixmapTemp1));
	baseUI->widButtomLeft->setPalette(palTemp1);
	baseUI->widButtomLeft->setMask(pixmapTemp1.mask());

	baseUI->widButtom->setStyleSheet("background-image:url(:img/ButtomBorder.png);");
	//baseUI->widButtomRight->setStyleSheet("background-image:url(:img/FooterRightSide.png);");
	baseUI->widButtomRight->setAutoFillBackground(true);
	QPixmap pixmapTemp2(":/img/FooterRightSide.png");
	QPalette palTemp2(baseUI->widButtomRight->palette());
	palTemp2.setBrush(QPalette::Window, QBrush(pixmapTemp2));
	baseUI->widButtomRight->setPalette(palTemp2);
	baseUI->widButtomRight->setMask(pixmapTemp2.mask());

	setIcon(":/img/title.png");
	baseUI->btnMin->setToolTip(tr("最小化"));
	baseUI->btnMax->setToolTip(tr("最大化"));
	baseUI->btnClose->setToolTip(tr("关闭窗体"));

	baseUI->widStatus->setVisible(false);	
}
/***************************************************************************
* 处理内容： 事件过滤器
* @param:	target		事件发送者
			event		事件描述
* @return    无
* @exception 
****************************************************************************/
bool FBase::eventFilter(QObject *target, QEvent *event)
{
	if(target == baseUI->labTitle && event->type() == QEvent::MouseButtonPress)
	{
		ReleaseCapture();
		SendMessage(this->winId(), WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
		return true;
	}
	if(target == baseUI->labTitle && event->type() == QEvent::MouseButtonDblClick)
	{
		showMax();
		return true;
	}
	return QWidget::eventFilter(target, event);
}
/***************************************************************************
* 处理内容： 以模态对话框形式打开
* @param:	
* @return    关闭的结果
* @exception 
****************************************************************************/
int FBase::showDialog()
{
	setWindowModality(Qt::ApplicationModal);
	show();
	dialogEventLoop = new QEventLoop;
	dialogEventLoop->exec();
	delete dialogEventLoop;
	return formResult;
}
/***************************************************************************
* 处理内容： 关闭模态对话框
* @param:	
* @return    
* @exception 
****************************************************************************/
void FBase::closeDialog(DialogResult result)
{
	formResult = result;
	if(dialogEventLoop)
	{
		dialogEventLoop->exit();
	}
	close();
}
/***************************************************************************
* 处理内容： 最大化窗体
* @param:	
* @return    
* @exception 
****************************************************************************/
void FBase::showMax()
{
	int iWidth = QApplication::desktop()->width();
	int iHeight = QApplication::desktop()->height();
	this->show();
	this->move(0,0);
	this->resize(iWidth, iHeight - 30);
}
/***************************************************************************
* 处理内容： 设置窗体标题
* @param:	strTitle	标题文本
* @return    
* @exception 
****************************************************************************/
void FBase::setTitle(const QString &strTitle)
{
	baseUI->labTitle->setText(strTitle);
	setWindowTitle(strTitle);
}
/***************************************************************************
* 处理内容： 设置标题栏上的图标
* @param:	strIconPath	图标路径
* @return    
* @exception 
****************************************************************************/
void FBase::setIcon(const QString &strIconPath)
{
	
	baseUI->labIcon->setVisible(true);

	QPixmap pixmapMid(18,18);
	//pixmapMid.scaledToWidth(25);
	//pixmapMid.scaledToHeight(20);
	
	pixmapMid.load(strIconPath);
	pixmapMid.scaled(18,18,Qt::KeepAspectRatio);
	baseUI->labIcon->setScaledContents(false);
	baseUI->labIcon->setPixmap(pixmapMid);

	setWindowIcon(QIcon(strIconPath));
	
}

/*************************************************************************
*处理内容：最大化
*@param：	
*@return：
*@exception 
**************************************************************************/
void FBase::btnMaxClicked()
{
	this->showMax();
}

/*************************************************************************
*处理内容：最小化
*@param：	
*@return：
*@exception 
**************************************************************************/
void FBase::btnMinClicked()
{
	this->showMinimized();
}

/*************************************************************************
*处理内容：关闭窗体
*@param：	
*@return：
*@exception 
**************************************************************************/
void FBase::btnCloseClicked()
{
	bool bCancel = false;
	emit beforeClose(bCancel);
	if(bCancel)
	{
		return;
	}
	if(dialogEventLoop)
	{
		dialogEventLoop->exit();
	}
	this->close();
}
/*************************************************************************
*处理内容：设置状态栏上文本
*@param：	
*@return：
*@exception 
**************************************************************************/
void FBase::setStatusBarText(const QString &strText, FBase::StatusBarArea barArea)
{
	switch(barArea)
	{
	case FBase::BarLeft:
		baseUI->labBarLeft->setText(strText);
		break;
	case FBase::BarRight:
		baseUI->labBarRight->setText(strText);
		break;
	default:
		break;
	}
	baseUI->widStatus->setVisible(true);
}