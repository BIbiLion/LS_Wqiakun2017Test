#ifndef SENDEMAIL_H
#define SENDEMAIL_H

#include "email.h"

#include <QtGui/QWidget>
#include "ui_sendemail.h"

class SendEmail : public QWidget
{
	Q_OBJECT

public:
	SendEmail(QWidget *parent = 0, Qt::WFlags flags = 0);
	~SendEmail();

	public slots:
		void on_pushButtonCommit_clicked();

private:
	Ui::SendEmailClass ui;

	Email m_Email;
};

#endif // SENDEMAIL_H
