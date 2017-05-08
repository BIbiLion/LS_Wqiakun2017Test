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
	bool eventFilter(QObject *target, QEvent *event);    //�¼�������
	void resizeEvent(QResizeEvent *aEvent);

private:
	void initView();                             //��ʼ������
	void connectToRemote();						 //����Զ�̷�����
	QString ftpCodeToGBKCode(const QString &ftpString);
	QString gbkCodeToFtpCode(const QString &gbkCode);
	QString getFileSize(qint64 iSize);

	int FindFileLocal(const QString & sComFilePath , const QString & sCurrentPath);    //���ļ��ϴ�����
	bool FindFileFtp(const QString & rootPath);			//ftp���ļ��б���

	bool uploadFile(QString pcPath);
private slots:
		void ftpCommandStarted(int state);
		void ftpCommandFinished(int id, bool error);

		void btnPublishClicked();
		void btnReflashClicked();
		//��������һ��Ŀ¼����ť�ĵ����¼��ۺ���
		void btnBackClicked();	
		//���������ϵ��ļ���ӵ�Tree Widget��
		void addToList(QUrlInfo urlInfo);

		//˫��һ��Ŀ¼ʱ��ʾ������
		void processItem(QTreeWidgetItem *item, int col);
		//���½�����
		void updateDataTransferProgress(qint64 done, qint64 total);

		//FTP�½��ļ���
		void addNewDir();
		//FTP�ϴ��ļ�
		void uploadFile();
		//FTPɾ���ļ�
		void deleteFile();
		//FTP�������ļ�
		void renameFile();
		//FTP�����ļ�
		void downloadFile();
				

		//�ϴ������ļ����ļ��У���ͬ�ļ�һ���ϴ�
		void uploadFold();			
		//ɾ�������ļ���Ŀ¼��
		void deleteFold(QTreeWidgetItem *item);
		//�����ļ��У��������������ļ�
		void downloadFold();

private:
	Ui::FtpClientClass m_ui;

	QMenu *m_mMenu;            //�һ��Ĳ˵�
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

	QString sPath;		//���ļ�������
	QString sName;
	QString SNm;
	bool add;			//�Ƿ��������ļ��ϴ�
	QVector<QString> m_fold;                   //ֻ����ļ���Ŀ¼
	QVector<QString>::iterator iter_fold;
	QHash<QString , QString> m_record;		//��ӦĿ¼�����ļ���������
	QHash<QString , QString>::iterator iter_record;
	QString pcPath , ftpPath;                     //�ڶ����ڵ�ȫ���ϴ�ű���·�� �� FTP·��

	QTreeWidgetItem *sysItem;  //ɾ���ļ����õ���  ��¼��Ҫ��ɾ��Ŀ¼������TreeWidget�е�λ��  ��Ҫȫ�ֵ�����Ϊ Ҫ��QFtp���źŷ�����������ʹ��
	QString sysText;   //ɾ���ļ����õ��� ��¼ɾ��ʱ��ѡ��Ŀ¼������  ͬ��
	bool fold;   //ɾ���ļ����õ���   ����ɾ��Ϊtrue  δɾ����ɾ������Ϊfalse
	int depth;   //Ŀ¼�����
};

#endif // FTPCLIENT_H
