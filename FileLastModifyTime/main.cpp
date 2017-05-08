
/*#include <QCoreApplication>
#include <QFileInfo>
#include <QFile>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    return a.exec();
}
*/



#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/utime.h>
#include <time.h>


//可以修改文件的最后存取时间   文件数据的最后修改时间
int main( void )
{
    struct tm tma = {0}, tmm = {0};
    struct utimbuf ut;

    tma.tm_hour = 12;
    tma.tm_isdst = 0;
    tma.tm_mday = 15;
    tma.tm_min = 0;
    tma.tm_mon = 0;
    tma.tm_sec = 0;
    tma.tm_year = 103;

    tmm.tm_hour = 12;
    tmm.tm_isdst = 0;
    tmm.tm_mday = 15;
    tmm.tm_min = 0;
    tmm.tm_mon = 0;
    tmm.tm_sec = 0;
    tmm.tm_year = 102;

    ut.actime = mktime(&tma);//  文件数据的最后访问时间
    ut.modtime = mktime(&tmm);//  文件数据的最后修改时间

    if( utime( "d:/test.txt", &ut ) == -1 )
        perror( "_utime failed\n" );
    else
        printf( "File time modified\n" );
    return 0;
}

 /*

//可以修改存在的文件的时间
#include  <iostream>
#include <windows.h>
using namespace std;
//DEFINES-= UNICODE
int main(int argc, char* argv[])
{
    SYSTEMTIME	t;
    FILETIME	ft;
    HANDLE		file;
    GetSystemTime(&t);
    t.wYear = 2012;
    SystemTimeToFileTime(&t, &ft);
    std::string _filePath("d:/test.txt");
    file = CreateFile(_filePath.c_str(), GENERIC_READ | GENERIC_WRITE,0,
        0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    SetFileTime(file, &ft, &ft, &ft);//文件创建时间  上一次访问时间 最后一次修改时间
    CloseHandle(file);
    return 0;
}

*/

/*可以获得文件的时间，但是修改失败
//这是我的源代码 就是通过第一个参数的修改时间 修改第二个文件参数的修改时间
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;
int main( int argc,char* argv[] )
{
    if(argc<2)
    {
        cout<<"No arguments"<<endl;
      //  exit(1);
    }

    string file1,file2;
//    file1=argv[1];
//    file2=argv[2];

    file1="C:/test.apk";
     file2="C:/config.ini";

    struct stat buf1;
    struct stat buf2;
    int result1,result2;
    //获得文件状态信息
    result1 =stat( file1.c_str(), &buf1 );
    result2 =stat( file2.c_str(), &buf2 );
    //显示文件状态信息
    if( result1 != 0 )
        perror( "显示文件状态信息出错" );
    else
    {
        cout<<"最后修改日期1:"<<ctime(&buf1.st_mtime);
    }

    //显示文件状态信息
    if( result1 != 0 )
        perror( "显示文件状态信息出错" );
    else
    {
        cout<<"最后修改日期2:"<<ctime(&buf2.st_mtime);
    }

    buf2.st_mtime=buf1.st_mtime;
    cout<<"最后修改日期3:"<<ctime(&buf2.st_mtime);
    return 0;
}
*/
