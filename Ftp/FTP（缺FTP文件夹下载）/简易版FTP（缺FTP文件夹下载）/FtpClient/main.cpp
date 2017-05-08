#include "ftpclient.h"
#include <QtGui/QApplication>
#include <QTextCodec>
#include <QMessageBox>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QTextCodec *textCodec = QTextCodec::codecForLocale();
	QTextCodec::setCodecForTr(textCodec);
	QTextCodec::setCodecForCStrings(textCodec);
	QTextCodec::setCodecForLocale(textCodec);

	FtpClient w;
	w.show();
	return a.exec();
}
