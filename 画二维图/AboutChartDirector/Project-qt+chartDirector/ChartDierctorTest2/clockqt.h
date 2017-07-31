#ifndef CLOCKQT_H
#define CLOCKQT_H

#include <QWidget>
#include <QTimer>
class Clockqt : public QWidget
{
    Q_OBJECT
public:
    explicit Clockqt(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);
signals:

public slots:
private:
    QTimer *timer;

};

#endif // CLOCKQT_H
