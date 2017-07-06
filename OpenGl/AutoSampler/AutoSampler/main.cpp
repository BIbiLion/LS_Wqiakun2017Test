#include "AutoSampler.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AutoSampler w;
    w.show();

    return a.exec();
}
