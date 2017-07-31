#ifndef TESTCHARDIRECT2_H
#define TESTCHARDIRECT2_H

#include <QtWidgets/QWidget>
#include "ui_testchardirect2.h"


#include "chartdir.h"
#include "qchartviewer.h"
class testCharDirect2 : public QWidget
{
	Q_OBJECT

public:
	testCharDirect2(QWidget *parent = 0);
	~testCharDirect2();

	 void	resizeEvent(QResizeEvent * event) ;
	 void draw(QWidget * w);
private:
	Ui::testCharDirect2Class ui;
	 XYChart *c;
	 QChartViewer * _vew;
};

#endif // TESTCHARDIRECT2_H
