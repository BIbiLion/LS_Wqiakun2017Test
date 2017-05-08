#include "sendemail.h"
#include "utils/utils.h"

SendEmail::SendEmail(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags),
	  m_Email(this)
{
	ui.setupUi(this);
}

SendEmail::~SendEmail()
{

}

void SendEmail::on_pushButtonCommit_clicked()
{
	if (ui.textEdit->toPlainText().isEmpty())
	{
		utils::MsgBox(tr("warning"), tr("content can't be empty!"));
		return ;
	}

	m_Email.SetUserInfo("你的邮箱帐号", "你的邮箱密码");
	m_Email.SetHost("smtp://smtp.163.com");		//你的邮箱smtp服务器地址
	m_Email.AddReceiver("<XXXX@163.com>");		//添加一个接受邮件者
	m_Email.SetSend("邮件主题", "邮件内容", "联系方式");
	m_Email.AddAttach("附件绝对路径");         //添加一个附件
	m_Email.start();
}