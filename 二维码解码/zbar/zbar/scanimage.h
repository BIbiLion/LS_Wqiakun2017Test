#ifndef SCANIMAGE_H
#define SCANIMAGE_H
#if defined(__cplusplus)
extern "C" {
#endif
#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <zbar.h>

/* to complete a runnable example, this abbreviated implementation of
 * get_data() will use libpng to read an image file. refer to libpng
 * documentation for details
 */
extern int scanimage (const char *name, const char *result);

#if defined(__cplusplus)
}
#endif

#endif // SCANIMAGE_H
