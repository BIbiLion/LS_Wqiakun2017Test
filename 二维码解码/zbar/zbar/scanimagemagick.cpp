#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QString>
#include "scanimagemagick.h"

int scanimagemagick(const char *filename, char *result)
{
#ifdef MAGICK_HOME
    // http://www.imagemagick.org/Magick++/
    //    under Windows it is necessary to initialize the ImageMagick
    //    library prior to using the Magick++ library
    Magick::InitializeMagick(MAGICK_HOME);
#endif

    // create a reader
    ImageScanner scanner;

    // configure the reader
    scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

    // obtain image data
    Magick::Image magick(filename);  // read an image file
    int width = magick.columns();   // extract dimensions
    int height = magick.rows();
    Magick::Blob blob;              // extract the raw data
    magick.modifyImage();
    magick.write(&blob, "GRAY", 8);
    const void *raw = blob.data();

    // wrap image data
    Image image(width, height, "Y800", raw, width * height);

    // scan the image for barcodes
    int n = scanner.scan(image);

    // extract results
    for(Image::SymbolIterator symbol = image.symbol_begin();
        symbol != image.symbol_end();
        ++symbol) {
        // do something useful with results
        cout << "decoded " << symbol->get_type_name()
             << " symbol \"" << QString(symbol->get_data().data()).toUtf8().data() << '"' << endl;
        sprintf(result, "%s:%s", symbol->get_type_name().data(), symbol->get_data().data());
    }

    // clean up
    image.set_data(NULL, 0);

    return(0);
}
