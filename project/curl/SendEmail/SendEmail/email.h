/************************************************************************/
/*	author : 狂风暴雨
 *	date   : 2013年11月14日 14:11:49
 *	desc   : 一份邮件的线程
 *	*/
/************************************************************************/

#ifndef EMAIL_H
#define EMAIL_H

#include "third_party/curl/curl.h"

#include <QStringList>
#include <QObject>
#include <QThread>

class Email : public QThread
{
	Q_OBJECT

public:
	Email(QObject *parent);
	~Email();

	//发送一封右键
	void run();

	void SetSend(const QString& subject, const QString& content, const QString& contact);

	//设置服务器地址
	void SetHost(const QString& host) { m_smtpServer = host;}
	QString Host() {return m_smtpServer;}

	//设置用户密码
	void SetUserName(const QString& name);
	QString UserName() { return m_userName;}

	void SetPassword(const QString& password);
	QString Password() { return m_passWord;}

	void SetUserInfo(const QString& name, const QString& password);

	//添加收信人,返回收信人数目
	int AddReceiver(const QString& receiver);
	QStringList Receiver() { return m_receiverList;}

	//附件
	int AddAttach(const QString& attachPath);
	QStringList Attachs() { return m_attachsList;}
	void RemoveAttach(int index);

	int Result() {return m_res;}

	//重置
	void Reset();
signals:
	void signalSendResult(int);

private:
	CURLcode m_res;

	//主题
	QString m_subject;
	QString m_content;
	QString m_contact;

	//用户密码
	QString m_userName;
	QString m_passWord;

	//stmp 服务器
	QString m_smtpServer;

	//接受人列表
	QStringList m_receiverList;

	//附件列表
	QStringList m_attachsList;

	static int ReadData(void* ptr, size_t size, size_t nmemb, void* userp);
};

#endif // EMAIL_H
