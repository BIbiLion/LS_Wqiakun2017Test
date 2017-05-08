#include <QFileInfo>
#include <QMessageBox>
#include <QString>

#include "utils/utils.h"

namespace utils
{
	std::string QStringToUtf8String(const QString& qString)
	{
		return qString.toUtf8().data();
	}

	QString PathFindFileName(const QString& path)
	{
		QFileInfo info(path);
		return info.fileName();
	}

	void MsgBox(const QString& title, const QString& content)
	{
		QMessageBox::information(NULL,title,content);
	}
}