#include <qstring.h>
#include <qwt_dial.h>
#include <qwt_dial_needle.h>
// 自定义 指南针的指针
class IndiacatorNeedle :public QwtDialNeedle
{
public:
    IndiacatorNeedle();
    IndiacatorNeedle(const QColor &);
    void setCenterAndRadiu(const QPointF &,const double &radius,const double &angle) ;
    void setInsiderCirAngle(double & angle);
protected:
    virtual void drawNeedle( QPainter *,
                             double length, QPalette::ColorGroup ) const;
private:
    QPoint m_center;
    double m_radius;
    double m_angle;
};




class SpeedoMeter: public QwtDial
{
public:
    SpeedoMeter( QWidget *parent = NULL );

    void setLabel( const QString & );
    QString label() const;


    void setAngle(const double & angle);
    void update_widget();
protected:

    virtual void drawScale( QPainter *,
                            const QPointF &center, double radius ) const;


    virtual void drawScaleContents( QPainter *painter,
                                    const QPointF &center, double radius ) const;

public:
    QPointF m_center;
    double m_radius;
private:
    QString d_label;
    double m_angle;//横滚角的 角度 内圆
public:
    IndiacatorNeedle *m_needle;

};
