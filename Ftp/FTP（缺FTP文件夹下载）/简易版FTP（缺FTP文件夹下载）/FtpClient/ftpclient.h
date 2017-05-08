#ifndef FTPCLIENT_H
#define FTPCLIENT_H

#include <QtGui/QDialog>
#include "ui_ftpclient.h"
#include <QMenu>
#include <QEvent>
#include <QFile>
#include <QList>
#include <QShowEvent>
#include <QFtp>
#include <QHash>
#include <QVector>
#include "ui_frmEditName.h"


#define ftpIP		"127.0.0.1"
#define ftpPort		21
#define ftpUser		"anonymous"
#define ftpPassword ""


class FEditName : public QDialog
{
	Q_OBJECT
public:
	FEditName(int key, const QString &strInfo = QString());
	~FEditName();

	QString retString;
private:
	Ui::frmEditName *m_ui;
	private slots:
		void btnSureClicked();
		void btnCancelClicked();
protected:
	void showEvent(QShowEvent *aEvent);
};



class FtpClient : public QDialog
{
	Q_OBJECT

public:
	FtpClient(QWidget *parent = 0, Qt::WFlags flags = 0);
	~FtpClient();

protected:
	bool eventFilter(QObject *target, QEvent *event);    //事件过滤器
	void resizeEvent(QResizeEvent *aEvent);

private:
	void initView();                             //初始化界面
	void connectToRemote();						 //连接远程服务器
	QString ftpCodeToGBKCode(const QString &ftpString);
	QString gbkCodeToFtpCode(const QString &gbkCode);
	QString getFileSize(qint64 iSize);

	int FindFileLocal(const QString & sComFilePath , const QString & sCurrentPath);    //多文件上传函数
	bool FindFileFtp(const QString & rootPath);			//ftp端文件夹遍历

	bool uploadFile(QString pcPath);
private slots:
		void ftpCommandStarted(int state);
		void ftpCommandFinished(int id, bool error);

		void btnPublishClicked();
		void btnReflashClicked();
		//“返回上一级目录”按钮的单击事件槽函数
		void btnBackClicked();	
		//将服务器上的文件添加到Tree Widget中
		void addToList(QUrlInfo urlInfo);

		//双击一个目录时显示其内容
		void processItem(QTreeWidgetItem *item, int col);
		//更新进度条
		void updateDataTransferProgress(qint64 done, qint64 total);

		//FTP新建文件夹
		void addNewDir();
		//FTP上传文件
		void uploadFile();
		//FTP删除文件
		void deleteFile();
		//FTP重命名文件
		void renameFile();
		//FTP下载文件
		void downloadFile();
				

		//上传包含文件的文件夹，连同文件一并上传
		void uploadFold();			
		//删除包含文件的目录项
		void deleteFold(QTreeWidgetItem *item);
		//下载文件夹，包含其中所有文件
		void downloadFold();

private:
	Ui::FtpClientClass m_ui;

	QMenu *m_mMenu;            //右击的菜单
	QAction *m_actAddDir;
	QAction *m_actAddFile;
	QAction *m_actDelFile;
	QAction *m_actRename;
	QAction *m_actAddFold;
	QAction *m_actDownloadFile;
	QAction *m_actDownLoadFold;

	QFile *m_pFile;
	QString m_sCurrentPath;
	QHash<QString, bool> m_hIsDirectory;

private:
	QFtp *m_Ftp;

	QString sPath;		//多文件增加用
	QString sName;
	QString SNm;
	bool add;			//是否启动多文件上传
	QVector<QString> m_fold;                   //只存放文件夹目录
	QVector<QString>::iterator iter_fold;
	QHash<QString , QString> m_record;		//对应目录下有文件则存放在这
	QHash<QString , QString>::iterator iter_record;
	QString pcPath , ftpPath;                     //在对象内的全局上存放本地路径 和 FTP路径

	QTreeWidgetItem *sysItem;  //删除文件夹用到了  记录将要被删除目录项所在TreeWidget中的位置  需要全局的是因为 要在QFtp的信号反馈处理函数中使用
	QString sysText;   //删除文件夹用到了 记录删除时所选的目录项名称  同上
	bool fold;   //删除文件夹用到了   正在删除为true  未删除或删除结束为false
	int depth;   //目录项深度
};

#endif // FTPCLIENT_H
