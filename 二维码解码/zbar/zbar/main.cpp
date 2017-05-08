#include "qrcodezbar.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QRCodeZbar w;
    w.show();

    return a.exec();
}
