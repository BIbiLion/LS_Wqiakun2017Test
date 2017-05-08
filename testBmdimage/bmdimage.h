#ifndef __BMDIMAGE_H__
#define __BMDIMAGE_H__

extern unsigned short progress_layer[];//Progress

int  InitSatelliteImage();
void ExitSatelliteImage();
int  DrawSatelliteImage(void *ptr);
int  ShowProgressWhenLoadingImage(void *ptr);

#endif
