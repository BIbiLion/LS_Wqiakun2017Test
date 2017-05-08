/***************************************************************************
*EFB地面管理系统
*头文件
*处理内容：基本窗体
*@author：李七星
*@version：1.0,2011-07-21,新建
****************************************************************************/

#ifndef FBASE_H
#define FBASE_H


#include <QDialog>
#include <QEvent>
#include <QEventLoop>
#include "ui_frmBase.h"

enum DialogResult
{
	UnKnown = 0,
	Sure = 1,
	Cancel = 2
};

class FBase : public QWidget
{
	Q_OBJECT
public:
	enum StatusBarArea
	{
		BarLeft = 0,	//状态栏左边
		BarRight = 1	//状态栏右边
	};
	FBase(QWidget *parent = 0);
	virtual ~FBase();

	//以模式对话框打开
	int showDialog();
	//最大化窗体
	void showMax();
	//设置是否能够调整大小
	void setResize(bool isCanResize);
	//设置标题
	void setTitle(const QString &strTitle);
	//设置图标
	void setIcon(const QString &strIconPath);
	//设置状态栏显示
	void setStatusBarText(const QString &strText, StatusBarArea barArea);
private:
	bool m_bCanResize;
	void createView();
protected:
	Ui::frmBase *baseUI;
	DialogResult formResult;
	QEventLoop *dialogEventLoop;

	bool eventFilter(QObject *target, QEvent *event);
	void closeDialog(DialogResult result);

	
private slots:
	void btnMinClicked();
	void btnMaxClicked();
	void btnCloseClicked();
signals:
	void beforeClose(bool &isCancel);
};

#endif