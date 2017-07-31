#ifndef CHISTOGRAM_H
#define CHISTOGRAM_H

#include <QWidget>
#include "qchartviewer.h"
class Chistogram : public QWidget
{
    Q_OBJECT
public:
    explicit Chistogram(QWidget *parent = 0);
    ~Chistogram();
    void setImagePath(QString savepath);
signals:

public slots:
    void saveImage();
private:
     XYChart *m_xychart;
     QString m_imgsavePath;
};

#endif // CHISTOGRAM_H
