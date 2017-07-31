#ifndef XYLINEWIDGET_H
#define XYLINEWIDGET_H

#include <QWidget>
#include <QDateTime>
#include <qchartviewer.h>
#include <chartdir.h>
 const int sampleSizeW= 200;//取样点

class XYLineWidget : public QWidget
{
    Q_OBJECT
public:
    explicit XYLineWidget(QWidget *parent = 0);
    ~XYLineWidget();
signals:

public slots:
    void draw();
    void dateSave();
private:
    QDateTime m_dateTime;
    QChartViewer *m_chartView;
    XYChart * m_xyChart;


    double dataArray[sampleSizeW];
    double m_timeStamps[sampleSizeW];
};

#endif // XYLINEWIDGET_H
