#include <QCoreApplication>
#include "../CtpTradeApi.h"
#include <unistd.h>

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);
    CCtpTradeApi* pApi = new CCtpTradeApi("tcp://180.168.146.187:10000");
    sleep(3);
    pApi->ReqLogin("034964","a262482384","9999");


    while(1);
   // return a.exec();
}
