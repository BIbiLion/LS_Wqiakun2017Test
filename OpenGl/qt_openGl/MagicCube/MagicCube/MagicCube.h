#ifndef MAGICCUBE_H
#define MAGICCUBE_H

#include <QWidget>
#include <QKeyEvent>
#include "Cube.h"
#include "sttthread.h"

namespace Ui {
class MagicCube;
}

class MagicCube : public QWidget
{
    Q_OBJECT

public:
    explicit MagicCube(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent* e);
    ~MagicCube();

signals:
    void startRecord();
    void stopRecord();



private:
    Ui::MagicCube *ui;
    Cube magicCube;
    sttThread stt;
};

#endif // MAGICCUBE_H
