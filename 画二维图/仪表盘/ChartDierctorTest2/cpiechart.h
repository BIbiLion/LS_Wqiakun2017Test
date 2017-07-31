#ifndef CPIECHART_H
#define CPIECHART_H

#include <QWidget>
#include "qchartviewer.h"
class Cpiechart : public QWidget
{
    Q_OBJECT
public:
    explicit Cpiechart(QWidget *parent = 0);
    ~Cpiechart();
    void setImagePath(QString savepath);
signals:

public slots:
    void saveImage();
private:
    PieChart *m_piechart;
    QString m_imgsavePath;
};

#endif // CPIECHART_H
