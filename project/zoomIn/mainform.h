#ifndef MAINFORM_H
#define MAINFORM_H

#include <QWidget>

class MainForm : public QWidget
{
    Q_OBJECT

public:
    MainForm(QWidget *parent = 0);
    ~MainForm();
private:
    void showEvent(QShowEvent*e);
    bool eventFilter(QObject *o, QEvent *e);
    void mousePressEvent(QMouseEvent*e);
    void mouseMoveEvent(QMouseEvent*e);
    void mouseReleaseEvent(QMouseEvent*e);
    void paintEvent(QPaintEvent*);

public slots:
    void slotTriggered(QAction *ac);
    void leaveEvent(QEvent *e);
private:
    QPoint m_pos;
    QPixmap m_pix;
    QPixmap m_tempPix;
    bool m_isDis;
    qreal m_O, m_width,m_height;
    int _shapeMode;//形状模式
};

#endif // MAINFORM_H
