#ifndef CLINECHART_H
#define CLINECHART_H

#include <QWidget>
#include "qchartviewer.h"
class Clinechart : public QWidget
{
    Q_OBJECT
public:
    explicit Clinechart(QWidget *parent = 0);
    ~Clinechart();
    void setImagePath(QString savepath);
signals:

public slots:
    void saveImage();
private:
    XYChart *m_linechart;
    QString m_imgsavePath;
};

#endif // CLINECHART_H
