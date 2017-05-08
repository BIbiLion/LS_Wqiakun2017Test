#include "bmdsatellite.h"
#include "ltplugin_api.h"
#include "ltapi.h"
#include "ltplugin.h"

//#define OUTPUT_JPEG                  /* 调试开关：将卫星影像输出到jpeg文件中 */

CBmdSatelliteLayer::CBmdSatelliteLayer()
{
	m_hInst             = NULL;
	m_fnGetImage        = NULL;
	m_fnClearImageBuff  = NULL;
	m_fnBmdFini         = NULL;
}

CBmdSatelliteLayer::~CBmdSatelliteLayer()
{
	if(m_hInst)
	{
		if(m_fnClearImageBuff)
		{
			m_fnClearImageBuff();
		}
		if(m_fnBmdFini)
		{
			m_fnBmdFini();
		}
		FreeLibrary(m_hInst);
		m_hInst = NULL;
	}
}

int CBmdSatelliteLayer::InitDbPath()
{
	m_nMaxScaleIndex = 18;
	m_nMinScaleIndex = 1;

	const char *root = sgcGetRootDir();
	char path[MAX_PATHLENGTH] = {0};
	osstrncpy(path, root, MAX_PATHLENGTH - 1);
	osstrncat(path, "plugin\\bmdimage\\db\\", MAX_PATHLENGTH - 1);
	osstrncpy(m_dbpath, path, MAX_PATHLENGTH-1);	

	//动态加载链接库
	char appfile[MAX_PATHLENGTH] = {0};
	osstrncpy(appfile, root, MAX_PATHLENGTH - 1);
	osstrncat(appfile, "plugin\\bmdimage\\BMDReader.dll", MAX_PATHLENGTH - 1);
	wchar_t wappfile[MAX_PATHLENGTH] = {0};
	MultiByteToWideChar(NULL, NULL, appfile, MAX_PATHLENGTH, wappfile, MAX_PATHLENGTH);
	m_hInst=LoadLibrary(wappfile); 
	if(NULL == m_hInst)
		return -1;

	m_fnGetImage = (lpGetImage)myGetProcAddress(m_hInst,"GetImage");
	m_fnClearImageBuff = (lpClearImageBuff)myGetProcAddress(m_hInst,"ClearImageBuff");
	m_fnBmdFini = (lpBmdFini)myGetProcAddress(m_hInst,"BmdFini");

	int ret = -1;
	lpBmdInit Bmdinit = (lpBmdInit)myGetProcAddress(m_hInst,"BmdInit");
	ret = Bmdinit();
	if(ret != 0)
		return -1;

	lpOpenPath OpenPath = (lpOpenPath)myGetProcAddress(m_hInst,"OpenPath");	
	ret = OpenPath(m_dbpath);
	if(ret != 0)
		return -1;

	return 0;
}

static int jpegCount = 1;
gdiBitmap * CBmdSatelliteLayer::GetTileImage(int tiledX, int tiledY, int scaleIndex)
{
	if(NULL == m_fnGetImage)
		return NULL;

#	ifdef _DEBUG
	osprintk("GetTileImage: %d \n", jpegCount++);
#	endif

	unsigned long bufsize = 0;
	BYTE *buffer = m_fnGetImage(&bufsize,scaleIndex,tiledX,tiledY );
	if(NULL == buffer)
		return NULL;

	gdiBitmap *bmp = gdiCreateBitmapFromMemory(m_pgc, (const char *)buffer, bufsize, GDI_JPEG);

#ifdef OUTPUT_JPEG
	//lihuafeng add
	char fileName[256];
	ossnprintf(fileName, 256, "E:\\Program\\svn_smg_10\\SmartGuider10_hz\\bin\\jpeg\\out.%d.jpeg", jpegCount);
	osHandle pFile;
	pFile = osFopen(fileName,"wb");
	osFwrite((const char *)buffer, bufsize, pFile);
	osFclose(pFile);
#endif

	return bmp;
}

void CBmdSatelliteLayer::GetTileUrl(int tiledX, int tiledY, int scaleIndex, char *url)
{
	tiledX;
	tiledY;
	scaleIndex;
	url;
}

void CBmdSatelliteLayer::GetTileFileName(int tiledX, int tiledY, int scaleIndex, char *filename)
{
	tiledX;
	tiledY;
	scaleIndex;
	filename;
}

