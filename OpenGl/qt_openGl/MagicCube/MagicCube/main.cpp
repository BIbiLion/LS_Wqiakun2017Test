#include "MagicCube.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MagicCube w;
    w.show();

    return a.exec();
}
