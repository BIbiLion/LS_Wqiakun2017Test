#ifndef SVGWIDGET_H
#define SVGWIDGET_H

#include <QWidget>
#include <QtSvg/QSvgWidget>
#include <QPolygonF>

#include <QTimer>
#include <Qmap>
#include <QPair>


class SvgWidget : public QSvgWidget
{
    Q_OBJECT
    enum TypeShape
    {
        Cricule,
        Rect,
        Path
    };

public:
    explicit SvgWidget(QWidget *parent = 0);

    void mousePressEvent(QMouseEvent*e);
    void analyzeSVG(QString _filePath);
public slots:
    void showCurPos();
private:
     QTimer * m_timer;
     QMap<QString,QPolygonF> m_areaNamePolygon;//区域名 点坐标

     QMap<QPair<QString ,TypeShape> ,QPolygonF> m_ply;
      QPolygonF m_westAreaPolygon;
      double m_detle;//缩放比例
};

#endif // SVGWIDGET_H
