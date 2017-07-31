#ifndef CDEFINECHART_H
#define CDEFINECHART_H

#include <QWidget>
#include "qchartviewer.h"
#define DATA_NUM 8
class CDefineChart : public QWidget
{
    Q_OBJECT
public:
    explicit CDefineChart(QWidget *parent = 0);
    ~CDefineChart();
    BaseChart * showHistogram();
    BaseChart * showPieChart();
    BaseChart * showLineChart();
signals:

public slots:

private:
    XYChart *m_xychart;
    PieChart *m_piechart;
    XYChart *m_linechart;

    double data[DATA_NUM];
    const char *labels[DATA_NUM];
};

#endif // CDEFINECHART_H
