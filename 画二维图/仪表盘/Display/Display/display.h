#ifndef DISPLAY_H
#define DISPLAY_H

#include <QWidget>
#include "form_km.h"
#include <QTimer>
namespace Ui {
class Display;
}

class Display : public QWidget
{
    Q_OBJECT

public:
    explicit Display(QWidget *parent = 0);
    ~Display();
protected slots:
    void change_Speed();

private:
    Ui::Display *ui;
    Form_KM  *km;


    QTimer testTimer;
};

#endif // DISPLAY_H
