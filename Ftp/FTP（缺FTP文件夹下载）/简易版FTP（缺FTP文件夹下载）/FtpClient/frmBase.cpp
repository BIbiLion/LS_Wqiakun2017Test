
#include <Windows.h>
#include <QDesktopWidget>
#include <QBitmap>
#include "frmBase.h"

/***************************************************************************
* �������ݣ� ���캯��
* @param     parent		������
* @return    ��
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
* �������ݣ� ��������
* @param     parent		������
* @return    ��
* @exception 
****************************************************************************/
FBase::~FBase()
{
	delete baseUI;
}

/***************************************************************************
* �������ݣ� �����Ƿ�ɸı䴰���С������󻯡���С����ť�Ƿ�ɼ�
* @param     isCanResize		�Ƿ���Ըı��С
* @return    ��
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
* �������ݣ� ��ʼ������
* @param     
* @return    ��
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
	baseUI->btnMin->setToolTip(tr("��С��"));
	baseUI->btnMax->setToolTip(tr("���"));
	baseUI->btnClose->setToolTip(tr("�رմ���"));

	baseUI->widStatus->setVisible(false);	
}
/***************************************************************************
* �������ݣ� �¼�������
* @param:	target		�¼�������
			event		�¼�����
* @return    ��
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
* �������ݣ� ��ģ̬�Ի�����ʽ��
* @param:	
* @return    �رյĽ��
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
* �������ݣ� �ر�ģ̬�Ի���
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
* �������ݣ� ��󻯴���
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
* �������ݣ� ���ô������
* @param:	strTitle	�����ı�
* @return    
* @exception 
****************************************************************************/
void FBase::setTitle(const QString &strTitle)
{
	baseUI->labTitle->setText(strTitle);
	setWindowTitle(strTitle);
}
/***************************************************************************
* �������ݣ� ���ñ������ϵ�ͼ��
* @param:	strIconPath	ͼ��·��
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
*�������ݣ����
*@param��	
*@return��
*@exception 
**************************************************************************/
void FBase::btnMaxClicked()
{
	this->showMax();
}

/*************************************************************************
*�������ݣ���С��
*@param��	
*@return��
*@exception 
**************************************************************************/
void FBase::btnMinClicked()
{
	this->showMinimized();
}

/*************************************************************************
*�������ݣ��رմ���
*@param��	
*@return��
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
*�������ݣ�����״̬�����ı�
*@param��	
*@return��
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