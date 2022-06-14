#include "PlaydateMain.h"

PlaydateAPI* gpd = NULL;
#include "Defines.h"



///
static int update(__attribute__ ((unused)) void* ud)
{
	UpdateMicroCity();
	return 1;
}


///
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
		gpd->graphics->fillRect(0, 0, 400, 240, kColorBlack);
		InitializeMicroCity();
		gpd->system->setUpdateCallback(update, NULL);
	}

	return 0;
}


