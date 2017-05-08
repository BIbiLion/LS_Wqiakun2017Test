#ifndef SCANIMAGEMAGICK_H
#define SCANIMAGEMAGICK_H
#include <iostream>
#include <Magick++.h>
#include <zbar.h>
#define STR(s) #s
/*****************
 * yum install ImageMagick
 ***********/
using namespace std;
using namespace zbar;

int scanimagemagick (const char *filename, char *result);

#endif // SCANIMAGE_H
