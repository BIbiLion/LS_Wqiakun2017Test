#include <QCoreApplication>
#include <QDebug>
#include <QObject>


#include "testobject.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TestObject test;

    return a.exec();
}
