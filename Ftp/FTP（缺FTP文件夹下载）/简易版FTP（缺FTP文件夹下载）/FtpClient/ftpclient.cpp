#include "ftpclient.h"
#include <QDesktopWidget>
#include <QUrl>
#include <QProcess>
#include <Windows.h>
#include <QFileDialog>
#include <QKeySequence>
#include <QContextMenuEvent>
#include <QFile>
#include <QMessageBox>


#pragma  region ���Ʊ༭��
FEditName::FEditName(int key, const QString &strInfo)
{
	m_ui = new Ui::frmEditName;
	m_ui->setupUi(this);
	switch(key)
	{
	case 0:
		m_ui->labTitle->setText(tr("�ļ�����"));
		setWindowTitle(tr("�½��ļ���"));
		m_ui->txtContent->setText(tr("�½��ļ���"));
		break;
	case 1:
		m_ui->labTitle->setText(tr("�ļ���"));
		m_ui->txtContent->setText(strInfo);
		setWindowTitle(tr("������"));
		break;
	default:
		break;
	}
	connect(m_ui->btnSure, SIGNAL(clicked()), this, SLOT(btnSureClicked()));
	connect(m_ui->btnCancel, SIGNAL(clicked()), this, SLOT(btnCancelClicked()));
	connect(m_ui->txtContent, SIGNAL(returnPressed()), this, SLOT(btnSureClicked()));

	//	setResize(false);
	int iWidth = QApplication::desktop()->width();
	int iHight = QApplication::desktop()->height();
	int iFormWidth = 310;
	int iFormHeight = 140;
	setGeometry(iWidth / 2 - iFormWidth / 2, iHight / 2 - iFormHeight / 2, iFormWidth, iFormHeight);
}

/*************************************************************************
*�������ݣ���������
*@param��
*@return��
*@exception 
**************************************************************************/
FEditName::~FEditName()
{
	delete m_ui;
}

void FEditName::showEvent(QShowEvent *aEvent)
{
	m_ui->txtContent->setFocus();
	m_ui->txtContent->selectAll();
}

/*************************************************************************
*�������ݣ�ȷ����ť
*@param��
*@return��
*@exception 
**************************************************************************/
void FEditName::btnSureClicked()
{
	if(m_ui->txtContent->text().length() == 0)
	{
		return;
	}
	retString = m_ui->txtContent->text();
	done(this->Accepted);
}

/*************************************************************************
*�������ݣ�ȡ���¼�
*@param��
*@return��
*@exception 
**************************************************************************/
void FEditName::btnCancelClicked()
{
	done(this->Rejected);
}
#pragma endregion


FtpClient::FtpClient(QWidget *parent, Qt::WFlags flags)
: QDialog(parent, flags)
{
	m_ui.setupUi(this);
	initView();

	connectToRemote();

	connect(m_ui.treeIssue, SIGNAL(itemActivated(QTreeWidgetItem *, int)),
		this, SLOT(processItem(QTreeWidgetItem *, int)));
	connect(m_ui.btnBack, SIGNAL(clicked()), this, SLOT(btnBackClicked()));
	connect(m_ui.btnReflash, SIGNAL(clicked()), this, SLOT(btnReflashClicked()));
	connect(m_ui.btnPublish, SIGNAL(clicked()), this, SLOT(btnPublishClicked()));

	sysItem = NULL;
	fold = false;
	add = false;
	depth = 0;
}

FtpClient::~FtpClient()
{
	if (NULL != m_mMenu)
	{
		delete m_mMenu;
	}
}

void FtpClient::initView()
{
	m_ui.treeIssue->setRootIsDecorated(false);
	m_ui.treeIssue->setHeaderLabels(QStringList() << tr("����") << tr("��С") << tr("�޸�����"));
	m_ui.treeIssue->header()->setStretchLastSection(false);
	m_ui.btnBack->setEnabled(false);
	m_ui.procBar->setVisible(false);

	m_mMenu = new QMenu(m_ui.treeIssue);
	m_actAddDir = m_mMenu->addAction(QIcon(":/img/add.png"),tr("�½��ļ���"), this, SLOT(addNewDir()), QKeySequence(Qt::CTRL + Qt::Key_A));
	m_actAddFile = m_mMenu->addAction(tr("�ϴ��ļ�"), this, SLOT(uploadFile()), QKeySequence(Qt::CTRL + Qt::Key_F));
	m_actAddFold = m_mMenu->addAction(tr("�ϴ��ļ���"), this, SLOT(uploadFold()), QKeySequence(Qt::CTRL + Qt::Key_W));
	m_actDelFile = m_mMenu->addAction(QIcon(":/img/delete.png"),tr("ɾ    ��"), this, SLOT(deleteFile()), QKeySequence(Qt::CTRL + Qt::Key_D));
	//	m_actDelFold = m_mMenu->addAction(QIcon(":/img/delete.png"),tr("ɾ��Ŀ¼��"), this, SLOT(delFold()), QKeySequence(Qt::CTRL + Qt::Key_C));
	m_actRename = m_mMenu->addAction(tr("�������ļ�"), this, SLOT(renameFile()), QKeySequence(Qt::CTRL + Qt::Key_R));
	m_actDownloadFile = m_mMenu->addAction(tr("�����ļ�"), this, SLOT(downloadFile()), QKeySequence(Qt::CTRL + Qt::Key_L));
	m_actDownLoadFold = m_mMenu->addAction(tr("�����ļ��м��������ļ�"), this, SLOT(downloadFold()), QKeySequence(Qt::Key_B));

	m_ui.treeIssue->installEventFilter(this);

	m_ui.treeIssue->setStyleSheet("QTreeWidget::item{height:20px;} ");
	m_mMenu->setStyleSheet("QMenu::item{height:30px;} ");
}

//Զ������
void FtpClient::connectToRemote()
{
	m_Ftp = new QFtp;
	connect(m_Ftp, SIGNAL(stateChanged(int)), this, SLOT(ftpCommandStarted(int)));
	connect(m_Ftp, SIGNAL(commandFinished(int,bool)),
		this, SLOT(ftpCommandFinished(int,bool)));
	connect(m_Ftp, SIGNAL(listInfo(QUrlInfo)),
		this, SLOT(addToList(QUrlInfo)));
	connect(m_Ftp, SIGNAL(dataTransferProgress(qint64, qint64)), this, SLOT(updateDataTransferProgress(qint64, qint64)));

	QUrl url( ftpIP );
	if (!url.isValid() || url.scheme().toLower() != QLatin1String("ftp")) 
	{
		m_Ftp->connectToHost( ftpIP, ftpPort);
		m_Ftp->login(ftpUser, ftpPassword);
	}
	else 
	{
		m_Ftp->connectToHost(url.host(), url.port(ftpPort));

		if (!url.userName().isEmpty())
			m_Ftp->login(QUrl::fromPercentEncoding(url.userName().toLatin1()), url.password());
		else
			m_Ftp->login(ftpIP, "ftp");
		if (!url.path().isEmpty())
			m_Ftp->cd(url.path());
	}
}

/*************************************************************************
*�������ݣ�FTP����״̬���
*@param��	state		״̬
*@return��
*@exception 
**************************************************************************/
void FtpClient::ftpCommandStarted(int state)
{
	switch(state)
	{
	case QFtp::Connecting:
		m_ui.stateLabel->setText(tr("������"));
		break;
	case QFtp::Connected:
		m_ui.stateLabel->setText(tr("������"));
		break;
	case QFtp::LoggedIn:
		m_ui.stateLabel->setText(tr("�ѵ�¼"));
		break;
	case QFtp::Unconnected:
		m_ui.stateLabel->setText(tr("δ����"));
		break;
	case QFtp::HostLookup:		
		break;
	case  QFtp::Get:
		m_ui.stateLabel->setText( tr("������"));
		break;
	default:
		break;
	}
}
/*************************************************************************
*�������ݣ��¼�������
*@param��	target		�¼�������
event		�¼�����
*@return��
*@exception 
**************************************************************************/
bool FtpClient::eventFilter(QObject *target, QEvent *event)
{
	if(target == m_ui.treeIssue && event->type() == QEvent::ContextMenu)
	{
		if(m_ui.treeIssue->selectedItems().count() == 0)
		{
			m_actDelFile->setEnabled(false);
			m_actRename->setEnabled(false);
			m_actDownloadFile->setEnabled(false);
			m_actDownLoadFold->setEnabled(true);
		}
		else
		{
			m_actDelFile->setEnabled(true);
			m_actRename->setEnabled(true);
			m_actDownloadFile->setEnabled(true);
			m_actDownLoadFold->setEnabled(false);
		}

		QContextMenuEvent *menuEvent = (QContextMenuEvent *)event;
		m_mMenu->popup(menuEvent->globalPos());		
		return true;
	}
	return QWidget::eventFilter(target, event);
}

/*************************************************************************
*�������ݣ���С�ı��¼�
*@param��	aEvent	�¼�����
*@return��
*@exception 
**************************************************************************/
void FtpClient::resizeEvent(QResizeEvent *aEvent)
{
	int iWidth = m_ui.treeIssue->width();
	m_ui.treeIssue->setColumnWidth(0, (iWidth / 3) * 2);
	m_ui.treeIssue->setColumnWidth(1, 80);
	m_ui.treeIssue->setColumnWidth(2, 150);
}

/*************************************************************************
*�������ݣ���������ˢ��
*@param��
*@return��
*@exception 
**************************************************************************/
void FtpClient::btnPublishClicked()
{
	if(QMessageBox::question(this,tr("��Ϣ��ʾ"), tr("ȷ��Ҫ����������ǰ�ĵ���\r\n�����ĵ�����ÿ���賿3���Զ�������"),
		QMessageBox::Yes | QMessageBox::Cancel) == QMessageBox::Yes)
	{
		//	if(ASILibrary::publishDoc())
		{
			QMessageBox::information(this, tr("��Ϣ��ʾ"), tr("�����ɹ���"));
		}
		//	else
		{	
			QMessageBox::information(this, tr("��Ϣ��ʾ"), tr("Զ�̷�����ִ�нű�ʧ�ܣ�\r\n����ϵ�ۺϷ���ϵͳ������Ա"));
		}
	}
}

/*************************************************************************
*�������ݣ����ذ�ť���
*@param��	
*@return��
*@exception 
**************************************************************************/
void  FtpClient::btnBackClicked()
{
	m_ui.treeIssue->clear();
	m_hIsDirectory.clear();
	m_sCurrentPath = m_sCurrentPath.left(m_sCurrentPath.lastIndexOf('/'));
	if (m_sCurrentPath.isEmpty()) 
	{
		m_ui.btnBack->setEnabled(false);
		m_Ftp->cd("/");
	} 
	else 
	{
		m_Ftp->cd(gbkCodeToFtpCode(m_sCurrentPath));
	}
	m_Ftp->list();
}

/*************************************************************************
*�������ݣ�ˢ��
*@param��
*@return��
*@exception 
**************************************************************************/
void FtpClient::btnReflashClicked()
{
	m_ui.treeIssue->clear();
	QFtp::State eState = m_Ftp->state();
	if(eState == QFtp::Unconnected || eState == QFtp::HostLookup)
	{
		m_Ftp->connectToHost(ftpIP, ftpPort);
		m_Ftp->login(ftpUser,ftpPassword);
	}
	else
	{
		m_Ftp->list();
	}
}

/*************************************************************************
*�������ݣ�ftp������ɻص�����
*@param��	id		��ʶ
error	�Ƿ���ִ���
*@return��
*@exception 
**************************************************************************/
void FtpClient::ftpCommandFinished(int id, bool error)
{
	if(m_Ftp->currentCommand() == QFtp::Login)
	{
		m_Ftp->list();
	}
	if(m_Ftp->currentCommand() == QFtp::List)
	{
		if(fold)   //��ѡFTP��������ʱ
		{
			QTreeWidgetItem *item ;
			item = m_ui.treeIssue->topLevelItem(0);
			if (0 == item)
			{
				btnBackClicked();
				depth--;             //���ص�FTPĿ¼�����һ��ʱ Ŀ¼���-1
			}
			else
			{
				//	QString str = item->text(0);
				if (0 != depth)         //��δ���ص�ɾ��ʱ��ѡ����һ��ʱ
				{
					deleteFold(item);             //�����ĳһ������ɾ����������Ŀ¼Ϊ�� ������ֹɾ��  ����δ����
				}
				else
				{
					int j = 0 ;
					QTreeWidgetItem *item ;
					item = m_ui.treeIssue->topLevelItem( j );
					QString str = item->text(0);
					while(1)              //�����ص���ѡɾ��Ŀ¼���ڼ�ʱ ������һ��ֱ���ҵ�����ѡ��������ͬ���� ��ɾ��֮
					{
						item = m_ui.treeIssue->topLevelItem( j );
						QString str = item->text(0);
						if (0 == QString::compare(str , sysText))   //��Ҫ��һ��Ŀ¼�µ���Ŀ¼�����������   ��������Ѿ���������
						{
							deleteFold(item);
							fold = false;     //���ݹ���ԭ��ѡFTPĿ¼��ʱ
							QMessageBox::information(this, tr("��Ϣ��ʾ"), tr("ɾ���ɹ���"));
							if (sysItem)
							{
								delete sysItem;
								sysItem = NULL;
							}
							break;
						}
						j++;
					}

				}

			}
		}
	}
	if(m_Ftp->currentCommand() == QFtp::Mkdir)
	{
		if (!add)            //�˷���˸
		{
			m_ui.treeIssue->clear();
		}
		m_Ftp->list();
		if (add)
		{
			QString createFoldPath;
			iter_fold++;
			if (iter_fold != m_fold.end())
			{
				createFoldPath = *iter_fold;
				m_Ftp->mkdir(gbkCodeToFtpCode(createFoldPath));
			}
			else
			{
				if (m_record.isEmpty())
				{
					m_ui.treeIssue->clear();
				}
				m_fold.clear();                  //�ļ��д����������մ���ļ���·����������
				if ( !m_record.isEmpty() )
				{
					//�ϴ��ļ�
					pcPath = iter_record.key();
					ftpPath = iter_record.value();        //����FTPĿ¼��ϵ���ĸ�Ŀ¼��
					iter_record++;
					m_Ftp->cd(gbkCodeToFtpCode(ftpPath));//�����ļ����ļ��������ļ�ʱ�ȴ򿪶�Ӧ��Ҫ�ϴ�����FTP����Ŀ¼��
				}
				else
				{
					add = false;
					//QMessageBox::information(this, tr("��Ϣ��ʾ"), tr("�ϴ��ɹ���"));
				}
			}
		}
	}
	if(m_Ftp->currentCommand() == QFtp::Rename)
	{
		m_ui.treeIssue->clear();
		m_Ftp->list();
	}
	if(m_Ftp->currentCommand() == QFtp::Rmdir)
	{
		if(!error)
		{
			m_ui.treeIssue->clear();
			m_Ftp->list();
		}
		else
		{
			//	if(QMessageBox::Yes==QMessageBox::question(this, tr("��Ϣ��ʾ"), tr("ɾ��ʧ��:��������Ƿ�Ҫ����ִ��ɾ��?"),QMessageBox::Yes | QMessageBox::Cancel))
			{
				//���ĳһ��Ŀ¼����ɾ��������Ŀ¼��
				fold = true;    //�����ѡFTPĿ¼�������ʱ
				processItem(sysItem , 0);   //ģ��˫�����οؼ�
				depth++;                         //FTPĿ¼������һ��ʱ Ŀ¼���+1
			}
		}
	}
	if (m_Ftp->currentCommand() == QFtp::Remove)
	{
		if(!error)
		{
			m_ui.treeIssue->clear();
			m_Ftp->list();
		}
		else
		{
			QMessageBox::information(this, tr("��Ϣ��ʾ"), tr("ɾ��ʧ�ܣ�"));
		}
	}
	if (m_Ftp->currentCommand() == QFtp::Cd)
	{
		if (true == add && false == m_record.isEmpty())    //�����ļ����ļ��������ļ�ʱ�ȴ򿪶�Ӧ��Ҫ�ϴ�����FTP����Ŀ¼��
		{
			uploadFile(pcPath);                                               //�������ļ��д�����Ϻ��ύ�ĵ�һ���ļ�������ִ��,ִ��ȫ���ļ�����
		}
		else if (true == add && true == m_record.isEmpty())
		{
			add = false;
			btnReflashClicked();
			//	QMessageBox::information(this, tr("��Ϣ��ʾ"), tr("�ϴ��ɹ���"));
		}
	}
	if(m_Ftp->currentCommand() == QFtp::Put)
	{
		if(!error)
		{
			if (!add)
			{
				m_ui.treeIssue->clear();
				m_Ftp->list();
			}

			if (add)
			{
				if (iter_record != m_record.end())     //������ļ����������δ�����������ʱ
				{
					pcPath = iter_record.key();
					ftpPath = iter_record.value();        //����FTPĿ¼��ϵ���ĸ�Ŀ¼��
					iter_record++;
					m_Ftp->cd(gbkCodeToFtpCode(ftpPath));
				}
				else
				{
					m_record.clear();                         //����ļ���Ϊ�´��ϴ��ļ�����׼��
					iter_record = m_record.begin();
					bool a = m_record.isEmpty();
					m_Ftp->cd(gbkCodeToFtpCode(m_sCurrentPath));
				}
			}
		}
		m_ui.procBar->setVisible(false);
		/*if(m_pFile)
		{
		m_pFile->close();
		delete m_pFile;
		}*/
	}
	if(m_Ftp->currentCommand() == QFtp::Get)
	{
		if(!error)
		{
			m_pFile->close();
			delete m_pFile;
			m_ui.stateLabel->setText( tr("�������"));
		}
	}
}

/*************************************************************************
*�������ݣ�˫������
*@param��	item	
col		��
*@return��
*@exception 
**************************************************************************/
void FtpClient::processItem(QTreeWidgetItem *item, int col)
{
	QString name = item->text(0);
	if (m_hIsDirectory.value(name))			//�������ļ��Ǹ�Ŀ¼�����
	{
		m_ui.treeIssue->clear();
		m_hIsDirectory.clear();
		m_sCurrentPath += '/';
		m_sCurrentPath += name;
		m_Ftp->cd(gbkCodeToFtpCode(name));
		m_Ftp->list();
		m_ui.btnBack->setEnabled(true);
	}
}

/*************************************************************************
*�������ݣ����Ŀ¼�б�
*@param��	urlInfo		Ŀ¼·����Ϣ
*@return��
*@exception 
**************************************************************************/
void FtpClient::addToList(QUrlInfo urlInfo)
{
	if (!add)   //�˷���˸
	{
		QTreeWidgetItem *item = new QTreeWidgetItem;

		QString str = ftpCodeToGBKCode(urlInfo.name());
		item->setText(0, str);
		item->setText(1, getFileSize(urlInfo.size()));
		item->setText(2, urlInfo.lastModified().toString("yyyy-MM-dd HH:mm"));
		item->setTextAlignment(1, Qt::AlignRight | Qt::AlignVCenter);

		QPixmap pixmap(urlInfo.isDir() ? "img/folder.png" : "img/file.png");
		item->setIcon(0, pixmap);

		m_hIsDirectory[str] = urlInfo.isDir();
		m_ui.treeIssue->addTopLevelItem(item);
	}
}

/*************************************************************************
*�������ݣ��ļ��������
*@param��	done	��ִ����
total	�ܹ���
*@return��
*@exception 
**************************************************************************/
void FtpClient::updateDataTransferProgress(qint64 readBytes, qint64 totalBytes)
{
	if(readBytes == totalBytes)
	{
		m_ui.procBar->setVisible(false);
	}
	m_ui.procBar->setRange(0, totalBytes);
	m_ui.procBar->setValue(readBytes);
	m_ui.procBar->setVisible(true);
}

/*************************************************************************
*�������ݣ�ȡ���ļ���С
*@param��iSize �ļ���С����λB
*@return��
*@exception 
**************************************************************************/
QString FtpClient::getFileSize(qint64 iSize)
{
	int iCalCount = 0;
	QString strUnit;
	float fSize = iSize;

	if(iSize == 0)
	{
		return "";
	}

	while (fSize > 1024)
	{
		fSize = fSize / 1024;
		iCalCount++;
	}
	switch(iCalCount)
	{
	case 0:
		return "1 KB";
		break;
	case 1:
		strUnit = " KB";
		break;
	case 2:
		strUnit = " MB";
		break;
	default:
		break;
	}
	return QString::number(fSize, 'f', 0) + strUnit;
}
/*************************************************************************
*�������ݣ��½��ļ���
*@param��	
*@return��
*@exception 
**************************************************************************/
void  FtpClient::addNewDir()
{
	FEditName editDialog(0);
	if(QDialog::Accepted == editDialog.exec())
	{
		m_Ftp->mkdir(gbkCodeToFtpCode(m_sCurrentPath + "/" + editDialog.retString));
	}
}

/*************************************************************************
*�������ݣ��ϴ��ļ�
*@param��	
*@return��
*@exception 
**************************************************************************/
void  FtpClient::uploadFile()
{
	QString strFileName = QFileDialog::getOpenFileName(this, tr("ѡ���ϴ��ļ�"), "E:\\", "*.*");
	if(strFileName.length() == 0)
	{
		return;
	}

	m_pFile = new QFile(strFileName);
	if (!m_pFile->open(QFile::ReadOnly))
	{
		m_pFile->close();
		QMessageBox::information(this, tr("��Ϣ��ʾ"), tr("���ļ�ʧ�ܣ�"));
		return;
	}
	int i = strFileName.lastIndexOf('/');
	QString strName = strFileName.right(strFileName.length() - i - 1);
	m_Ftp->put(m_pFile, gbkCodeToFtpCode(strName));
}

/*************************************************************************
*�������ݣ�ɾ���ļ�
*@param��	
*@return��
*@exception 
**************************************************************************/
void  FtpClient::deleteFile()
{
	//	QString strFileName = m_ui.treeIssue->currentItem()->text(0);

	QTreeWidgetItem *item = m_ui.treeIssue->currentItem ();
	sysItem = item;
	sysText = sysItem->text(0);
	int index = m_ui.treeIssue->indexOfTopLevelItem (item);
	item = m_ui.treeIssue->topLevelItem(index);
	QString strFileName = item->text(0);  //��ǰ��
	QString strPath = gbkCodeToFtpCode(m_sCurrentPath) + "/" + gbkCodeToFtpCode(strFileName);
	bool bSure = false;

	if(m_hIsDirectory.value(strFileName))
	{
		if(QMessageBox::question(this,tr("��Ϣ��ʾ"), tr("ȷ��Ҫɾ���ļ���\"") + strFileName + "\"��",
			QMessageBox::Yes | QMessageBox::Cancel) == QMessageBox::Yes)
		{
			m_Ftp->rmdir(strPath);
		}
	}
	else
	{
		if(QMessageBox::question(this,tr("��Ϣ��ʾ"), tr("ȷ��Ҫɾ���ļ�\"") + strFileName + "\"��",
			QMessageBox::Yes | QMessageBox::Cancel) == QMessageBox::Yes)
		{
			m_Ftp->remove(strPath);

		}
	}
}

/*************************************************************************
*�������ݣ�ɾ�������ļ�֮�ļ���
*@param��	item ��������ؼ��е�ĳ����
*@return��
*@exception 
**************************************************************************/
void FtpClient::deleteFold(QTreeWidgetItem *item)
{
	sysItem = item;
	QString strFileName = item->text(0);		//��ǰ��
	QString strPath = gbkCodeToFtpCode(m_sCurrentPath) + "/" + gbkCodeToFtpCode(strFileName);
	if(m_hIsDirectory.value(strFileName))
	{
		m_Ftp->rmdir(strPath);
	}
	else
	{
		m_Ftp->remove(strPath);
	}
}

void FtpClient::downloadFold()
{
	//0���FTPָ�����Ƿ�Ϊ�ļ�Ŀ¼
	QTreeWidgetItem *item = m_ui.treeIssue->currentItem ();
	sysItem = item;
	sysText = sysItem->text(0);
	int index = m_ui.treeIssue->indexOfTopLevelItem (item);
	item = m_ui.treeIssue->topLevelItem(index);
	QString strFileName = item->text(0);  //��ǰ��
	QString strPath = gbkCodeToFtpCode(m_sCurrentPath) + "/" + gbkCodeToFtpCode(strFileName);
	bool bSure = false;

	if(m_hIsDirectory.value(strFileName))
	{
		//�ж�Ϊ�ļ���
	}
	else
	{
		//�ж�Ϊ�ļ�
		return;
	}

	//1����ѡ�����ļ��е�Ŀ¼
	QString localDir = QFileDialog::getExistingDirectory(this, tr("ѡ���ļ��д��Ŀ¼"),
		"/home",
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (localDir.isEmpty())
	{
		return;
	}

	//2��FTP�˱����ļ��м��ļ���������ļ��У�����ֱ���ڱ����½���������ļ�����
	//?




}


/*************************************************************************
*�������ݣ��������ļ�
*@param��	
*@return��
*@exception 
**************************************************************************/
void FtpClient::renameFile()
{
	QString strFileName = m_ui.treeIssue->currentItem()->text(0);
	FEditName editDialog(1, strFileName);
	if(QDialog::Accepted == editDialog.exec())
	{
		m_Ftp->rename(gbkCodeToFtpCode(strFileName),gbkCodeToFtpCode(editDialog.retString));
	}
}

void FtpClient::downloadFile()
{
	QString strFileName = m_ui.treeIssue->currentItem()->text(0);

	QString dir = QFileDialog::getExistingDirectory(this, tr("ѡ�������ļ����Ŀ¼"),
		"/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	strFileName = dir + QDir::separator() + strFileName;
	//
	if (QFile::exists(strFileName)) {
		QMessageBox msgBox;
		msgBox.setText(QString("�ļ��ڵ�ǰ·�����Ѵ��� %1 ").arg(strFileName));
		msgBox.setInformativeText("�Ƿ�ֱ�Ӹ��Ǳ����ļ�");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Yes);

		switch (msgBox.exec())
		{
		case QMessageBox::Yes:
			{
				if(!QFile::remove(strFileName))
				{
					QMessageBox::information(this, tr("ɾ��ʧ��"),
						tr("�޷�ɾ���ļ� %1.").arg(strFileName));
					return;
				}
				break;
			}
		case QMessageBox::Cancel:
			return;
			break;
		default:
			return;
		}
	}

	m_pFile = new QFile(strFileName);
	if (!m_pFile->open(QIODevice::WriteOnly)) {
		QMessageBox::information(this, tr("FTP"),
			tr("�޷������ļ� %1: %2.")
			.arg(strFileName).arg(m_pFile->errorString()));
		m_pFile->close();
		return;
	}

	m_Ftp->get(gbkCodeToFtpCode(m_ui.treeIssue->currentItem()->text(0)), m_pFile);

}

void FtpClient::uploadFold()
{
	QString strFilePath = QFileDialog::getExistingDirectory(this, tr("ѡ����Ҫ�ϴ����ļ���"), "E:\\", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (0 == strFilePath.length())          //ȡ��������
	{
		return ;
	}
	int i = strFilePath.lastIndexOf('/');
	QString strTempName = strFilePath.right(strFilePath.length() - i - 1);
	//	m_Ftp->mkdir(gbkCodeToFtpCode(m_sCurrentPath + "/" + strName));

	sPath = m_sCurrentPath;
	QString sCurrentPath = m_sCurrentPath + "/" + strTempName;
	m_fold.append(sCurrentPath);        //����ѡ�ļ���FTP·���������Ӧ������
	FindFileLocal(strFilePath, sCurrentPath);

	add = true;
	iter_fold = m_fold.begin();
	iter_record = m_record.begin();
	QString foldPath;
	if (NULL != iter_fold)
	{
		foldPath = *iter_fold;
	}
	else
		return;
	m_Ftp->mkdir(gbkCodeToFtpCode(foldPath));

	m_sCurrentPath = sPath;
}

int FtpClient::FindFileLocal(const QString & sComFilePath , const QString & sCurrentPath)
{
	QDir dir(sComFilePath);
	if (!dir.exists()) 
	{
		return -1;
	}
	//ȡ�����е��ļ����ļ���������ȥ��.��..���ļ��У�����QTĬ���еģ�
	dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);

	//�ļ�������
	dir.setSorting(QDir::DirsFirst);
	//ת����һ��list
	QFileInfoList list = dir.entryInfoList();
	if(list.size()< 1 ) 
	{
		return -1;
	}
	int i = 0 ;
	//�ݹ��㷨�ĺ��Ĳ���
	do
	{
		QFileInfo fileInfo = list.at(i);
		QString fileNm = fileInfo.fileName();
		//������ļ��У��ݹ�
		bool bisDir = fileInfo.isDir();
		if(bisDir) 
		{
			//��Ӹ����ļ���
			int i = fileNm.lastIndexOf('/');
			QString strName = fileNm.right(fileNm.length() - i - 1);
			QString currentPath = sCurrentPath + "/" + strName;
			QString currentFilePath = fileInfo.filePath();
			m_fold.append(currentPath);
			FindFileLocal(fileInfo.filePath() , currentPath);
		}
		else
		{
			QString str = fileNm.right(4);

			int i = fileNm.lastIndexOf('/');
			QString strName = fileNm.right(fileNm.length() - i - 1);
			strName = gbkCodeToFtpCode(strName);
			m_sCurrentPath = sCurrentPath;            //��Ӧ��FTP�е���֯�е�ĳ����֧·��
			i = sCurrentPath.lastIndexOf('/');
			QString name = sCurrentPath.right(sCurrentPath.length() - i - 1);
			//		m_Ftp->cd(gbkCodeToFtpCode(name));
			QString tempPath = sComFilePath + "/" + fileNm;           //ֱ���ļ���ȫ·��
			m_record[tempPath] = sCurrentPath;                        //PC���ļ�·����key ��Ϊһ���ļ����¿����ж���ļ�

		}
		i++;
	} while(i < list.size());
	return 0;
}

bool FtpClient::FindFileFtp(const QString & rootPath)
{
	QDir dir(rootPath);
	if (!dir.exists())
		return false;

	dir.setFilter(QDir::Dirs|QDir::Files);
	dir.setSorting(QDir::DirsFirst);

	QFileInfoList list = dir.entryInfoList();
	int i = 0;
	bool bIsDir;

	do{
		QFileInfo fileInfo = list.at(i);
		if(fileInfo.fileName()=="."|fileInfo.fileName()=="..")
		{
			i++;
			continue;
		}
		bIsDir = fileInfo.isDir();
		if (bIsDir)
		{
			//nFiles++;                //���ļ����Խ��д���
			//fileInfo.size(),fileInfo.fileName(),fileInfo.path()
			FindFileFtp(fileInfo.filePath());
		}
		else
		{
			//nFiles++;
		}
		i++;
	}
	while(i<list.size());

	return true;
}

bool FtpClient::uploadFile(QString pcPath)
{
	if(pcPath.length() == 0)
	{
		return false;
	}
	QFile *m_pFile = new QFile(pcPath);
	if (!m_pFile->open(QFile::ReadOnly))
	{
		m_pFile->close();
		return false;
	}
	int i = pcPath.lastIndexOf('/');
	QString sFileName = pcPath.right(pcPath.length() - i - 1);
	sFileName = gbkCodeToFtpCode(sFileName);
	m_Ftp->put(m_pFile, sFileName);
	//	waitPutFile.lock();
	return true;
}

/*************************************************************************
*�������ݣ���FTP�����ʽת��ΪGBK
*@param��ftpString		��ת���ַ���
*@return��
*@exception 
**************************************************************************/
QString FtpClient::ftpCodeToGBKCode(const QString &ftpString)
{
	QString TempResult;   
	//1.��ȡ����   
	size_t InStrBufSize = ftpString.size() * sizeof(QChar);   
	char* InStrBuf = (char *)malloc(InStrBufSize);   
	if (InStrBuf == NULL)       
	{   
		return TempResult;   
	}   
	memcpy(InStrBuf, ftpString.data(), InStrBufSize);   
	//2.�������   
	size_t i;  
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN   
	//С�˶�����(����Intel��x86�ܹ�)   
	//ԭʼ�����±� 0 2 4 6 8 10   
	//�޸������±� 0 1 2 3 4 5   
	i= 0;   
	for (i = 0; i < InStrBufSize; i = i + 2)      
	{   
		InStrBuf[i >> 1] = InStrBuf[i];       
	}   
	memset(InStrBuf + (i >> 1), 0, i >> 1);  
#else   
	//��˶�����   
	//ԭʼ�����±� 1 3 5 7 9   
	//�޸������±� 0 1 2 3 4   
	i = 1;   
	for (i = 1; i <= InStrBufSize; i = i + 2)   
	{   
		InStrBuf[(i - 1) >> 1] = InStrBuf[i];   
	}   
	memset(InStrBuf + ((i - 1) >> 1), 0, (i - 1) >> 1);
#endif   
	//3.����������װ   
	TempResult = TempResult.fromAscii(InStrBuf);   
	free(InStrBuf);   
	return TempResult;   
}

/*************************************************************************
*�������ݣ���GBK�����ʽת��ΪFTP�����ʽ
*@param��gbkCode		��ת���ַ���
*@return��
*@exception 
**************************************************************************/
QString FtpClient::gbkCodeToFtpCode(const QString &gbkCode)
{
	QString TempResult;   
	//1.��ȡ����   
	size_t InStrBufSize = gbkCode.toAscii().size();   
	char* InStrBuf = (char *)malloc(InStrBufSize);//= InputStr.toAscii().data();   
	memset(InStrBuf, 0, InStrBufSize);   
	if (InStrBuf == NULL)   
	{   
		return TempResult;    
	}   
	memcpy(InStrBuf, gbkCode.toAscii().constData(), InStrBufSize);   
	//2.�����������   
	//��������СҪ��Ascii�ֽ�*2��������ֽ� ���ڱ�ʶ����ַ�����β  
	size_t OutStrBufSize = (InStrBufSize + 1) * sizeof(QChar);   
	quint16* OutStrBuf = (quint16 *)malloc(OutStrBufSize);   
	if (OutStrBuf == NULL)    
	{   
		return TempResult;   
	}   
	memset(OutStrBuf, 0, OutStrBufSize);   
	//3.���±���   
	size_t i = 0;   
	for(i = 0; i < InStrBufSize; i++)   
	{   
		OutStrBuf[i] = (quint8)InStrBuf[i];   
	}   
	//4.����������װ   
	TempResult = QString::fromUtf16(OutStrBuf);   
	free(InStrBuf);   
	free(OutStrBuf);   
	return TempResult;   
}