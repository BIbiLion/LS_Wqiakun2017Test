#ifndef METERWIDGET_H
#define METERWIDGET_H

#include <QWidget>
#include "chartdir.h"
#include "qchartviewer.h"
class MeterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MeterWidget(double startAngle,double endAngle,QWidget *parent = 0);
    ~MeterWidget();

    void setMetreValue(double Value=0.0);
signals:

public slots:
     void InitMeterWidget();
    void test_setValue();
private:
    AngularMeter * m_angularMeter;
    MeterPointer * m_meterPointer;


    QChartViewer * m_chartView;

    double m_startAngle;
    double m_endAngle;

    double m_value;
};

#endif // METERWIDGET_H
