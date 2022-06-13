#include "PlaydateMain.h"

PlaydateAPI* gpd = NULL;
#include "Defines.h"




static int update(__attribute__ ((unused)) void* ud)
{
	UpdateMicroCity();
	return 1;
}


static const LCDPattern bg = {
    0, 0xff, 0, 0xff,
	0, 0xff, 0, 0xff,
    0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff
};


extern "C" int eventHandler
(
	  PlaydateAPI* playdate
	, PDSystemEvent event
	, uint32_t arg
)
{
	if (event == kEventInitLua)
	{
		gpd = playdate;

		gpd->display->setRefreshRate(25); 
		gpd->graphics->fillRect(0, 0, 400, 240, (LCDColor)&bg);
		InitializeMicroCity();
		gpd->system->setUpdateCallback(update, NULL);
	}

	return 0;
}


