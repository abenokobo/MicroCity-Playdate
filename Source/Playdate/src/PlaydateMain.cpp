#include "PlaydateMain.h"


PlaydateAPI* gpd = NULL;



///
static int update(__attribute__ ((unused)) void* ud)
{
    MicroCity::GetInstance().Update();
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
        gpd->system->setUpdateCallback(update, NULL);

        MicroCity::GetInstance().Initialize();
    }
    else if (event == kEventPause)
    {
        MicroCity::GetInstance().OnPause();
    }
    else if (event == kEventTerminate)
    {
        MicroCity::Finalize();
    }

    return 0;
}


// undefined reference
extern "C" void __dso_handle(void)
{
}



