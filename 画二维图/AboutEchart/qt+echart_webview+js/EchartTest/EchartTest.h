#ifndef EChartTest_H
#define EChartTest_H

#include <QFutureWatcher>
#include <QtWidgets>

class EChartTest : public QObject
{
    Q_OBJECT
public:
    EChartTest(QObject * parent=0);
	~EChartTest();

public slots:
	QStringList GetMaxValue();
	QStringList GetMinValue();
	void submit();

private:
	int m_nBaseData;
};

#endif
