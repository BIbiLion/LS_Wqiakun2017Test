#ifndef TESTCOM_DIAL_THERMO_H
#define TESTCOM_DIAL_THERMO_H

#include <QWidget>
#include "mythermo.h"
namespace Ui {
class testCom_dial_thermo;
}

class testCom_dial_thermo : public QWidget
{
    Q_OBJECT

public:
    explicit testCom_dial_thermo(QWidget *parent = 0);
    ~testCom_dial_thermo();

private:
    Ui::testCom_dial_thermo *ui;
};

#endif // TESTCOM_DIAL_THERMO_H
