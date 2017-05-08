#include "CtpTradeApi.h"

#include "stdio.h"
#include <unistd.h>

int main()
{


	


	CCtpTradeApi* pApi = new CCtpTradeApi("tcp://180.168.146.187:10000");
	sleep(3);
	pApi->ReqLogin("034964","a262482384","9999");

	getchar();

	return 1;
}
