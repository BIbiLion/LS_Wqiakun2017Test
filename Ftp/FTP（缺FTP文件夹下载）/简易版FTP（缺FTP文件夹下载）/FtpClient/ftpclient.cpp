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


#pragma  region 名称编辑框
FEditName::FEditName(int key, const QString &strInfo)
{
	m_ui = new Ui::frmEditName;
	m_ui->setupUi(this);
	switch(key)
	{
	case 0:
		m_ui->labTitle->setText(tr("文件夹名"));
		setWindowTitle(tr("新建文件夹"));
		m_ui->txtContent->setText(tr("新建文件夹"));
		break;
	case 1:
		m_ui->labTitle->setText(tr("文件名"));
		m_ui->txtContent->setText(strInfo);
		setWindowTitle(tr("重命名"));
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
*处理内容：析构函数
*@param：
*@return：
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
*处理内容：确定按钮
*@param：
*@return：
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
*处理内容：取消事件
*@param：
*@return：
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
	m_ui.treeIssue->setHeaderLabels(QStringList() << tr("名称") << tr("大小") << tr("修改日期"));
	m_ui.treeIssue->header()->setStretchLastSection(false);
	m_ui.btnBack->setEnabled(false);
	m_ui.procBar->setVisible(false);

	m_mMenu = new QMenu(m_ui.treeIssue);
	m_actAddDir = m_mMenu->addAction(QIcon(":/img/add.png"),tr("新建文件夹"), this, SLOT(addNewDir()), QKeySequence(Qt::CTRL + Qt::Key_A));
	m_actAddFile = m_mMenu->addAction(tr("上传文件"), this, SLOT(uploadFile()), QKeySequence(Qt::CTRL + Qt::Key_F));
	m_actAddFold = m_mMenu->addAction(tr("上传文件夹"), this, SLOT(uploadFold()), QKeySequence(Qt::CTRL + Qt::Key_W));
	m_actDelFile = m_mMenu->addAction(QIcon(":/img/delete.png"),tr("删    除"), this, SLOT(deleteFile()), QKeySequence(Qt::CTRL + Qt::Key_D));
	//	m_actDelFold = m_mMenu->addAction(QIcon(":/img/delete.png"),tr("删除目录项"), this, SLOT(delFold()), QKeySequence(Qt::CTRL + Qt::Key_C));
	m_actRename = m_mMenu->addAction(tr("重命名文件"), this, SLOT(renameFile()), QKeySequence(Qt::CTRL + Qt::Key_R));
	m_actDownloadFile = m_mMenu->addAction(tr("下载文件"), this, SLOT(downloadFile()), QKeySequence(Qt::CTRL + Qt::Key_L));
	m_actDownLoadFold = m_mMenu->addAction(tr("下载文件夹及所有子文件"), this, SLOT(downloadFold()), QKeySequence(Qt::Key_B));

	m_ui.treeIssue->installEventFilter(this);

	m_ui.treeIssue->setStyleSheet("QTreeWidget::item{height:20px;} ");
	m_mMenu->setStyleSheet("QMenu::item{height:30px;} ");
}

//远程链接
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
*处理内容：FTP连接状态变更
*@param：	state		状态
*@return：
*@exception 
**************************************************************************/
void FtpClient::ftpCommandStarted(int state)
{
	switch(state)
	{
	case QFtp::Connecting:
		m_ui.stateLabel->setText(tr("连接中"));
		break;
	case QFtp::Connected:
		m_ui.stateLabel->setText(tr("已连接"));
		break;
	case QFtp::LoggedIn:
		m_ui.stateLabel->setText(tr("已登录"));
		break;
	case QFtp::Unconnected:
		m_ui.stateLabel->setText(tr("未连接"));
		break;
	case QFtp::HostLookup:		
		break;
	case  QFtp::Get:
		m_ui.stateLabel->setText( tr("下载中"));
		break;
	default:
		break;
	}
}
/*************************************************************************
*处理内容：事件过滤器
*@param：	target		事件发送者
event		事件参数
*@return：
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
*处理内容：大小改变事件
*@param：	aEvent	事件参数
*@return：
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
*处理内容：立即发布刷新
*@param：
*@return：
*@exception 
**************************************************************************/
void FtpClient::btnPublishClicked()
{
	if(QMessageBox::question(this,tr("信息提示"), tr("确定要立即发布当前文档吗？\r\n若否，文档将在每日凌晨3点自动发布。"),
		QMessageBox::Yes | QMessageBox::Cancel) == QMessageBox::Yes)
	{
		//	if(ASILibrary::publishDoc())
		{
			QMessageBox::information(this, tr("信息提示"), tr("发布成功！"));
		}
		//	else
		{	
			QMessageBox::information(this, tr("信息提示"), tr("远程服务器执行脚本失败！\r\n请联系综合服务系统管理人员"));
		}
	}
}

/*************************************************************************
*处理内容：返回按钮点击
*@param：	
*@return：
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
*处理内容：刷新
*@param：
*@return：
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
*处理内容：ftp命令完成回调函数
*@param：	id		标识
error	是否出现错误
*@return：
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
		if(fold)   //所选FTP项有子项时
		{
			QTreeWidgetItem *item ;
			item = m_ui.treeIssue->topLevelItem(0);
			if (0 == item)
			{
				btnBackClicked();
				depth--;             //返回到FTP目录项的上一级时 目录深度-1
			}
			else
			{
				//	QString str = item->text(0);
				if (0 != depth)         //尚未返回到删除时所选的那一层时
				{
					deleteFold(item);             //如果在某一级不能删除并且其下目录为空 即可终止删除  此项未加入
				}
				else
				{
					int j = 0 ;
					QTreeWidgetItem *item ;
					item = m_ui.treeIssue->topLevelItem( j );
					QString str = item->text(0);
					while(1)              //当返回到所选删除目录所在级时 遍历这一级直到找到和所选项名称相同的项 并删除之
					{
						item = m_ui.treeIssue->topLevelItem( j );
						QString str = item->text(0);
						if (0 == QString::compare(str , sysText))   //这要求一个目录下的子目录项不得有重名的   这个问题已经不存在了
						{
							deleteFold(item);
							fold = false;     //当递归至原所选FTP目录项时
							QMessageBox::information(this, tr("信息提示"), tr("删除成功！"));
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
		if (!add)            //克服闪烁
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
				m_fold.clear();                  //文件夹创建完毕则清空存放文件夹路径的数据区
				if ( !m_record.isEmpty() )
				{
					//上传文件
					pcPath = iter_record.key();
					ftpPath = iter_record.value();        //所在FTP目录体系中哪个目录下
					iter_record++;
					m_Ftp->cd(gbkCodeToFtpCode(ftpPath));//包含文件的文件夹中有文件时先打开对应于要上传到的FTP所在目录级
				}
				else
				{
					add = false;
					//QMessageBox::information(this, tr("信息提示"), tr("上传成功！"));
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
			//	if(QMessageBox::Yes==QMessageBox::question(this, tr("信息提示"), tr("删除失败:含有子项！是否要继续执行删除?"),QMessageBox::Yes | QMessageBox::Cancel))
			{
				//如果某一级目录不能删除则进入此目录下
				fold = true;    //如果所选FTP目录项含有子项时
				processItem(sysItem , 0);   //模拟双击树形控件
				depth++;                         //FTP目录进入下一级时 目录深度+1
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
			QMessageBox::information(this, tr("信息提示"), tr("删除失败！"));
		}
	}
	if (m_Ftp->currentCommand() == QFtp::Cd)
	{
		if (true == add && false == m_record.isEmpty())    //包含文件的文件夹中有文件时先打开对应于要上传到的FTP所在目录级
		{
			uploadFile(pcPath);                                               //在所有文件夹创建完毕后提交的第一个文件在这里执行,执行全部文件数次
		}
		else if (true == add && true == m_record.isEmpty())
		{
			add = false;
			btnReflashClicked();
			//	QMessageBox::information(this, tr("信息提示"), tr("上传成功！"));
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
				if (iter_record != m_record.end())     //当存放文件项的数据区未被遍历至最后时
				{
					pcPath = iter_record.key();
					ftpPath = iter_record.value();        //所在FTP目录体系中哪个目录下
					iter_record++;
					m_Ftp->cd(gbkCodeToFtpCode(ftpPath));
				}
				else
				{
					m_record.clear();                         //清空文件项为下次上传文件夹做准备
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
			m_ui.stateLabel->setText( tr("下载完毕"));
		}
	}
}

/*************************************************************************
*处理内容：双击处理
*@param：	item	
col		列
*@return：
*@exception 
**************************************************************************/
void FtpClient::processItem(QTreeWidgetItem *item, int col)
{
	QString name = item->text(0);
	if (m_hIsDirectory.value(name))			//如果这个文件是个目录，则打开
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
*处理内容：添加目录列表
*@param：	urlInfo		目录路径信息
*@return：
*@exception 
**************************************************************************/
void FtpClient::addToList(QUrlInfo urlInfo)
{
	if (!add)   //克服闪烁
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
*处理内容：文件传输进度
*@param：	done	已执行数
total	总共数
*@return：
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
*处理内容：取得文件大小
*@param：iSize 文件大小，单位B
*@return：
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
*处理内容：新建文件夹
*@param：	
*@return：
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
*处理内容：上传文件
*@param：	
*@return：
*@exception 
**************************************************************************/
void  FtpClient::uploadFile()
{
	QString strFileName = QFileDialog::getOpenFileName(this, tr("选择上传文件"), "E:\\", "*.*");
	if(strFileName.length() == 0)
	{
		return;
	}

	m_pFile = new QFile(strFileName);
	if (!m_pFile->open(QFile::ReadOnly))
	{
		m_pFile->close();
		QMessageBox::information(this, tr("信息提示"), tr("打开文件失败！"));
		return;
	}
	int i = strFileName.lastIndexOf('/');
	QString strName = strFileName.right(strFileName.length() - i - 1);
	m_Ftp->put(m_pFile, gbkCodeToFtpCode(strName));
}

/*************************************************************************
*处理内容：删除文件
*@param：	
*@return：
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
	QString strFileName = item->text(0);  //当前项
	QString strPath = gbkCodeToFtpCode(m_sCurrentPath) + "/" + gbkCodeToFtpCode(strFileName);
	bool bSure = false;

	if(m_hIsDirectory.value(strFileName))
	{
		if(QMessageBox::question(this,tr("信息提示"), tr("确定要删除文件夹\"") + strFileName + "\"吗？",
			QMessageBox::Yes | QMessageBox::Cancel) == QMessageBox::Yes)
		{
			m_Ftp->rmdir(strPath);
		}
	}
	else
	{
		if(QMessageBox::question(this,tr("信息提示"), tr("确定要删除文件\"") + strFileName + "\"吗？",
			QMessageBox::Yes | QMessageBox::Cancel) == QMessageBox::Yes)
		{
			m_Ftp->remove(strPath);

		}
	}
}

/*************************************************************************
*处理内容：删除包含文件之文件夹
*@param：	item 传入的树控件中的某个项
*@return：
*@exception 
**************************************************************************/
void FtpClient::deleteFold(QTreeWidgetItem *item)
{
	sysItem = item;
	QString strFileName = item->text(0);		//当前项
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
	//0检查FTP指定的是否为文件目录
	QTreeWidgetItem *item = m_ui.treeIssue->currentItem ();
	sysItem = item;
	sysText = sysItem->text(0);
	int index = m_ui.treeIssue->indexOfTopLevelItem (item);
	item = m_ui.treeIssue->topLevelItem(index);
	QString strFileName = item->text(0);  //当前项
	QString strPath = gbkCodeToFtpCode(m_sCurrentPath) + "/" + gbkCodeToFtpCode(strFileName);
	bool bSure = false;

	if(m_hIsDirectory.value(strFileName))
	{
		//判断为文件夹
	}
	else
	{
		//判断为文件
		return;
	}

	//1本地选择存放文件夹的目录
	QString localDir = QFileDialog::getExistingDirectory(this, tr("选择文件夹存放目录"),
		"/home",
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (localDir.isEmpty())
	{
		return;
	}

	//2在FTP端遍历文件夹及文件，如果是文件夹，可以直接在本地新建，如果是文件则复制
	//?




}


/*************************************************************************
*处理内容：重命名文件
*@param：	
*@return：
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

	QString dir = QFileDialog::getExistingDirectory(this, tr("选在下载文件存放目录"),
		"/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	strFileName = dir + QDir::separator() + strFileName;
	//
	if (QFile::exists(strFileName)) {
		QMessageBox msgBox;
		msgBox.setText(QString("文件在当前路径下已存在 %1 ").arg(strFileName));
		msgBox.setInformativeText("是否直接覆盖本地文件");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Yes);

		switch (msgBox.exec())
		{
		case QMessageBox::Yes:
			{
				if(!QFile::remove(strFileName))
				{
					QMessageBox::information(this, tr("删除失败"),
						tr("无法删除文件 %1.").arg(strFileName));
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
			tr("无法保存文件 %1: %2.")
			.arg(strFileName).arg(m_pFile->errorString()));
		m_pFile->close();
		return;
	}

	m_Ftp->get(gbkCodeToFtpCode(m_ui.treeIssue->currentItem()->text(0)), m_pFile);

}

void FtpClient::uploadFold()
{
	QString strFilePath = QFileDialog::getExistingDirectory(this, tr("选择需要上传的文件夹"), "E:\\", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (0 == strFilePath.length())          //取消即返回
	{
		return ;
	}
	int i = strFilePath.lastIndexOf('/');
	QString strTempName = strFilePath.right(strFilePath.length() - i - 1);
	//	m_Ftp->mkdir(gbkCodeToFtpCode(m_sCurrentPath + "/" + strName));

	sPath = m_sCurrentPath;
	QString sCurrentPath = m_sCurrentPath + "/" + strTempName;
	m_fold.append(sCurrentPath);        //将所选文件的FTP路径存放在相应数据区
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
	//取到所有的文件和文件名，但是去掉.和..的文件夹（这是QT默认有的）
	dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);

	//文件夹优先
	dir.setSorting(QDir::DirsFirst);
	//转化成一个list
	QFileInfoList list = dir.entryInfoList();
	if(list.size()< 1 ) 
	{
		return -1;
	}
	int i = 0 ;
	//递归算法的核心部分
	do
	{
		QFileInfo fileInfo = list.at(i);
		QString fileNm = fileInfo.fileName();
		//如果是文件夹，递归
		bool bisDir = fileInfo.isDir();
		if(bisDir) 
		{
			//添加各个文件夹
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
			m_sCurrentPath = sCurrentPath;            //对应于FTP中的组织中的某个分支路径
			i = sCurrentPath.lastIndexOf('/');
			QString name = sCurrentPath.right(sCurrentPath.length() - i - 1);
			//		m_Ftp->cd(gbkCodeToFtpCode(name));
			QString tempPath = sComFilePath + "/" + fileNm;           //直到文件的全路径
			m_record[tempPath] = sCurrentPath;                        //PC上文件路径做key 因为一个文件夹下可能有多个文件

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
			//nFiles++;                //对文件属性进行处理
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
*处理内容：将FTP编码格式转化为GBK
*@param：ftpString		待转化字符串
*@return：
*@exception 
**************************************************************************/
QString FtpClient::ftpCodeToGBKCode(const QString &ftpString)
{
	QString TempResult;   
	//1.提取数据   
	size_t InStrBufSize = ftpString.size() * sizeof(QChar);   
	char* InStrBuf = (char *)malloc(InStrBufSize);   
	if (InStrBuf == NULL)       
	{   
		return TempResult;   
	}   
	memcpy(InStrBuf, ftpString.data(), InStrBufSize);   
	//2.拆分数据   
	size_t i;  
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN   
	//小端对齐下(基于Intel的x86架构)   
	//原始数据下标 0 2 4 6 8 10   
	//修复数据下标 0 1 2 3 4 5   
	i= 0;   
	for (i = 0; i < InStrBufSize; i = i + 2)      
	{   
		InStrBuf[i >> 1] = InStrBuf[i];       
	}   
	memset(InStrBuf + (i >> 1), 0, i >> 1);  
#else   
	//大端对齐下   
	//原始数据下标 1 3 5 7 9   
	//修复数据下标 0 1 2 3 4   
	i = 1;   
	for (i = 1; i <= InStrBufSize; i = i + 2)   
	{   
		InStrBuf[(i - 1) >> 1] = InStrBuf[i];   
	}   
	memset(InStrBuf + ((i - 1) >> 1), 0, (i - 1) >> 1);
#endif   
	//3.数据重新组装   
	TempResult = TempResult.fromAscii(InStrBuf);   
	free(InStrBuf);   
	return TempResult;   
}

/*************************************************************************
*处理内容：将GBK编码格式转化为FTP编码格式
*@param：gbkCode		待转化字符串
*@return：
*@exception 
**************************************************************************/
QString FtpClient::gbkCodeToFtpCode(const QString &gbkCode)
{
	QString TempResult;   
	//1.提取数据   
	size_t InStrBufSize = gbkCode.toAscii().size();   
	char* InStrBuf = (char *)malloc(InStrBufSize);//= InputStr.toAscii().data();   
	memset(InStrBuf, 0, InStrBufSize);   
	if (InStrBuf == NULL)   
	{   
		return TempResult;    
	}   
	memcpy(InStrBuf, gbkCode.toAscii().constData(), InStrBufSize);   
	//2.建立输出缓冲   
	//输出缓冲大小要比Ascii字节*2后多两个字节 用于标识输出字符串结尾  
	size_t OutStrBufSize = (InStrBufSize + 1) * sizeof(QChar);   
	quint16* OutStrBuf = (quint16 *)malloc(OutStrBufSize);   
	if (OutStrBuf == NULL)    
	{   
		return TempResult;   
	}   
	memset(OutStrBuf, 0, OutStrBufSize);   
	//3.重新编码   
	size_t i = 0;   
	for(i = 0; i < InStrBufSize; i++)   
	{   
		OutStrBuf[i] = (quint8)InStrBuf[i];   
	}   
	//4.数据重新组装   
	TempResult = QString::fromUtf16(OutStrBuf);   
	free(InStrBuf);   
	free(OutStrBuf);   
	return TempResult;   
}