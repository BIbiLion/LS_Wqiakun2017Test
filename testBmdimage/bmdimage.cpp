#include "bmdsatellite.h"
#include "bmdimage.h"
#include "ltapi.h"
#include "ltplugin.h"

CMercatorSatelliteLayer * g_pSatelliteImage = NULL;
volatile int has_init_bmdimage = 0;

unsigned short progress_layer[] = {0x50,0x72,0x6f,0x67,0x72,0x65,0x73,0x73,0};//Progress

unsigned long init_bmdimage_thread(void * )
{
#	if defined  ( BMD_SATELLITE_IMAGE )
	g_pSatelliteImage = new CBmdSatelliteLayer;
#	elif defined ( GOOGLE_SATELLITE_IMAGE )
	g_pSatelliteImage = new CGoogleSatelliteLayer;
#	endif

#ifdef _DEBUG
	osSleep(3000);
#endif

	if ( g_pSatelliteImage->Init(uicGetGraphicsContext()) != 0 )
	{
		delete g_pSatelliteImage;
		g_pSatelliteImage = NULL;
		return 0;
	}

	has_init_bmdimage = 1;
	
	return 1;
}

int ShowProgressWhenLoadingImage(void *ptr)
{
	if (0 == has_init_bmdimage && (*(int *)ptr) != 2)
	{
		char filename[256] = {0};
		const char * root_dir = sgcGetRootDir();	
		const char * png_file = "plugin\\bmdimage\\loading.png";
		osstrncpy(filename, root_dir, 256);
		osstrncat(filename, png_file, 256);

		unsigned short filefullname[MAX_PATHLENGTH] = {0};
		osMultiByteToWideChar(filefullname, filename, MAX_PATHLENGTH);

		gdiGraphicsContext * gc = uicGetGraphicsContext();
		gdiIconList * iconList = gdiCreateIconlistFromFile(gc,filefullname, 715);
		gdiPoint pt = {1, 383};
		gdiDrawIconlist(gc, iconList, &pt, 0);
	}
	else
	{
		ScMdDeleteLayer(progress_layer);
	}	

	return 0;
}

int InitSatelliteImage()
{
	has_init_bmdimage = 0;	

	osCreateThread(init_bmdimage_thread,NULL,OS_PRIORITY_NORMAL);	

	return 0;
}

int DrawSatelliteImage(void *ptr)
{	
	if(0 == has_init_bmdimage /*|| NULL == g_pSatelliteImage*/)
		return -1;

	gdiRect rt = { 0 }; 
	rt.right  = gdiGraphicsContextWidth(uicGetGraphicsContext()); 
	rt.bottom = gdiGraphicsContextHeight(uicGetGraphicsContext()); 

	SMG_Rect rect = { 0 };
	ScGetMapRect(&rect);
	int scale = ScGetScale();

	long cenx,ceny;
	ScGetEyePointXY(&cenx, &ceny);	
	int angle = ScGetMapAngle();

	bool ret = g_pSatelliteImage->Draw(rt, rect, scale, cenx, ceny, angle);
	return ret ? 0 : -1;
}

void ExitSatelliteImage()
{
	if(g_pSatelliteImage)
	{
		delete g_pSatelliteImage;
		g_pSatelliteImage = NULL;
	}
}
