
#ifndef UTILS_H
#define UTIIS_H

class QString;
class QFileInfo;

namespace utils
{
	std::string QStringToUtf8String(const QString&);

	QString PathFindFileName(const QString&);

	void MsgBox(const QString& title, const QString& content);
}

#endif