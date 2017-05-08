/***************************************************************************
*EFB�������ϵͳ
*ͷ�ļ�
*�������ݣ���������
*@author��������
*@version��1.0,2011-07-21,�½�
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
		BarLeft = 0,	//״̬�����
		BarRight = 1	//״̬���ұ�
	};
	FBase(QWidget *parent = 0);
	virtual ~FBase();

	//��ģʽ�Ի����
	int showDialog();
	//��󻯴���
	void showMax();
	//�����Ƿ��ܹ�������С
	void setResize(bool isCanResize);
	//���ñ���
	void setTitle(const QString &strTitle);
	//����ͼ��
	void setIcon(const QString &strIconPath);
	//����״̬����ʾ
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