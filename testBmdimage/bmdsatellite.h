#ifndef _BMDSATELLITE_H_
#define _BMDSATELLITE_H_

#include "mercatorsatellite.h"
#include "windows.h"

#ifdef _WIN32_WCE
	#define myGetProcAddress GetProcAddressA
#else
	#define myGetProcAddress GetProcAddress
#endif

//dll输出函数声明
typedef int(__cdecl *   lpBmdInit)();
typedef int(__cdecl *   lpOpenPath)(char*);
typedef BYTE *(__cdecl *lpGetImage)(DWORD*,BYTE,DWORD,DWORD);
typedef void(__cdecl *  lpClearImageBuff)();
typedef int(__cdecl *   lpBmdFini)();

/*
 * @class 天目卫星影像
 */
class CBmdSatelliteLayer : public CMercatorSatelliteLayer
{
public:
	CBmdSatelliteLayer();
	virtual ~CBmdSatelliteLayer();

protected:
	virtual int         InitDbPath();
	virtual gdiBitmap * GetTileImage(int tiledX, int tiledY, int scaleIndex);
	virtual void        GetTileFileName(int tiledX, int tiledY, int scaleIndex, char *filename);
	virtual void        GetTileUrl(int tiledX, int tiledY, int scaleIndex, char *url);	

private:
	HINSTANCE           m_hInst;
	lpGetImage          m_fnGetImage;
	lpClearImageBuff    m_fnClearImageBuff;
	lpBmdFini           m_fnBmdFini;
};

#endif