#include <QApplication>
#include "glwidget.h"

int main(int argc, char **argv)
{
    QApplication a(argc, argv);

    GLWidget glw;
    glw.resize(600, 600);
    glw.show();

    return a.exec();
}






