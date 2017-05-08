#include <QCoreApplication>
#include "easy.h"
#include "curlbuild.h"
#include "curl.h"
#include <QFile>
#include <QString>
#include <QDebug>


struct FtpFile {
    const char *filename;
    FILE *stream;
};
int curlWriteFunction(void *buffer, size_t size, size_t nmemb, void *stream)
{

    struct FtpFile *out=(struct FtpFile *)stream;
    if(out && !out->stream) {
        /* open file for writing */
        out->stream=fopen(out->filename, "wb");
        if(!out->stream)
            return -1; /* failure, can't open file to write */
    }
    return fwrite(buffer, size, nmemb, out->stream);

    /*
    QFile* file = static_cast<QFile*>(stream);
    if(!file->isOpen()){
        file->open(QFile::ReadWrite | QIODevice::Text);
    }
    return file->write((char*)buffer,nmemb);
*/
}



void function()
{
    CURLcode ret;
    curl_global_init(CURL_GLOBAL_ALL);
    CURL* curl = curl_easy_init();

    struct FtpFile ftpfile={
        "yourfile.bin", /* name to store the file as if successful */
        NULL
    };

    QString remoteFile("ftp://10.0.0.249/ftpsget.c");
    curl_easy_setopt(curl,CURLOPT_URL,remoteFile.toStdString().c_str());
    curl_easy_setopt(curl,CURLOPT_USERPWD,"wqiankun:123456");
    ret = curl_easy_setopt(curl,CURLOPT_WRITEDATA,&ftpfile);
    ret = curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,curlWriteFunction);


    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
    // curl_easy_setopt(curl,CURLOPT_TRANSFERTEXT,1);

    curl_easy_setopt(curl,CURLOPT_VERBOSE,1);
    ret = curl_easy_perform(curl);//执行curl动作

    curl_easy_cleanup(curl);
    if(ftpfile.stream)
        fclose(ftpfile.stream); /* close the local file */

    curl_global_cleanup();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    function();

    return a.exec();
}
