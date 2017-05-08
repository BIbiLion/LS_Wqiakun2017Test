#include "bmdimage.h"
#include "ltplugin_api.h"
#include "ltapi.h"
#include "ltplugin.h"

DECLARE_API

int g_type = 1;

void OnOk(void *ptr)
{
	//bmdÓ°Ïñ
	unsigned short layername[] = {0x62,0x6d,0x64,0x5f71,0x50cf,0};
	switch(g_type)
	{
	case 0:
		{
			ScMdAddLayer(MD_LAYER_BASE, MD_FEATURE_UNKNOWN, layername, DrawSatelliteImage, NULL);
			ScShowRoadNet(false);
		}
		break;
	case 1:
		{
			ScMdAddLayer(MD_LAYER_BASE, MD_FEATURE_UNKNOWN, layername, DrawSatelliteImage, NULL);
			ScShowRoadNet(true);
		}
		break;
	case 2:
		{
			ScMdDeleteLayer(layername);
			ScShowRoadNet(true);
		}
		break;
	default:
		break;
	}
	
	sgcOnGotoMap(ptr);	

	ScStatusCommonSetLong("BMDIMAGE","TYPE",g_type);
}

void OnShow(void *ptr)
{
	g_type = 0;
	uicSetButtonChecked(2, 1);
	uicSetButtonChecked(4, 0);
	uicSetButtonChecked(6, 0);
}

void OnShowWithRoadNet(void *ptr)
{
	g_type = 1;
	uicSetButtonChecked(2, 0);
	uicSetButtonChecked(4, 1);
	uicSetButtonChecked(6, 0);
}

void OnClose(void *ptr)
{
	g_type = 2;
	uicSetButtonChecked(2, 0);
	uicSetButtonChecked(4, 0);
	uicSetButtonChecked(6, 1);
}

void LoadUserSet()
{
	g_type = 1;
	ScStatusCommonGetLong("BMDIMAGE","TYPE",&g_type);

	uicSetButtonChecked(2, 0 == g_type);
	uicSetButtonChecked(4, 1 == g_type);
	uicSetButtonChecked(6, 2 == g_type);

	OnOk(NULL);
}

extern "C" int init_plugin(LtPlugin p, LtApi *api)
{
	SET_API(api)
	
	if (InitSatelliteImage() != 0)
		return -1;

	ScMdAddLayer(MD_LAYER_APP, MD_FEATURE_UNKNOWN, progress_layer, 
				 ShowProgressWhenLoadingImage, &g_type);

	LoadUserSet();		
	
	return 0;
}

extern "C" void exit_plugin()
{
	ExitSatelliteImage();
	CLEAR_API
}

extern "C" int entry_plugin()
{	
	uicPushDialogXml("plugin\\bmdimage\\bmdimage.xml");
    uicSetTopCallbackId(8, "cbClick", OnOk);
    uicSetTopCallbackId(2, "cbClick", OnShow);
    uicSetTopCallbackId(4, "cbClick", OnShowWithRoadNet);
    uicSetTopCallbackId(6, "cbClick", OnClose);

	switch(g_type)
	{
	case 0:
		OnShow(NULL);
		break;
	case 1:
		OnShowWithRoadNet(NULL);
		break;
	case 2:
		OnClose(NULL);
		break;
	default:
		break;
	}
	return 0;
}

extern "C" int plugin_level()
{
	return 3;
}


