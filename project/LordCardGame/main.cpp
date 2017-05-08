#include "MainFrame.h"
#include <QtGui/QApplication>
#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QVector>
#include <QSet>
#include "Strategy.h"
#include <QFont>
#include <QTranslator>

void myMessageOutput(QtMsgType type, const char *msg)
{
	switch (type) {
	 case QtDebugMsg:
		 fprintf(stderr, "Debug: %s\n", msg);
		 break;
	 case QtWarningMsg:
		 fprintf(stderr, "Warning: %s\n", msg);
		 break;
	 case QtCriticalMsg:
		 fprintf(stderr, "Critical: %s\n", msg);
		 break;
	 case QtFatalMsg:
		 fprintf(stderr, "Fatal: %s\n", msg);
		 abort();
	}
}


int main(int argc, char *argv[])
{
	qInstallMsgHandler(myMessageOutput);
	QApplication a(argc, argv);

	QTranslator translator;
	translator.load("zh.qm", QApplication::applicationDirPath());
	a.installTranslator(&translator);

	MainFrame w;
	w.setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
	w.show();

	return a.exec();
}
