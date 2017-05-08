/************************************************************************/
/*	author : ��籩��
 *	date   : 2013��11��14�� 14:11:49
 *	desc   : һ���ʼ����߳�
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

	//����һ���Ҽ�
	void run();

	void SetSend(const QString& subject, const QString& content, const QString& contact);

	//���÷�������ַ
	void SetHost(const QString& host) { m_smtpServer = host;}
	QString Host() {return m_smtpServer;}

	//�����û�����
	void SetUserName(const QString& name);
	QString UserName() { return m_userName;}

	void SetPassword(const QString& password);
	QString Password() { return m_passWord;}

	void SetUserInfo(const QString& name, const QString& password);

	//���������,������������Ŀ
	int AddReceiver(const QString& receiver);
	QStringList Receiver() { return m_receiverList;}

	//����
	int AddAttach(const QString& attachPath);
	QStringList Attachs() { return m_attachsList;}
	void RemoveAttach(int index);

	int Result() {return m_res;}

	//����
	void Reset();
signals:
	void signalSendResult(int);

private:
	CURLcode m_res;

	//����
	QString m_subject;
	QString m_content;
	QString m_contact;

	//�û�����
	QString m_userName;
	QString m_passWord;

	//stmp ������
	QString m_smtpServer;

	//�������б�
	QStringList m_receiverList;

	//�����б�
	QStringList m_attachsList;

	static int ReadData(void* ptr, size_t size, size_t nmemb, void* userp);
};

#endif // EMAIL_H
