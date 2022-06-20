#include "MicroCity.h"
#include "Interface.h"
#include "Game.h"


/*
    CAPI 'playdate->system->addMenuItem' callback function is not called,
    so implemented in Lua.
*/
//#define USE_CAPI_MENU



///
static const char* DATA_FILE_NAME = "city.dat";

///
static const int STATE_SIZE = sizeof(GameState);


///
std::shared_ptr<MicroCity> MicroCity::m_gspInstance;



/*
    Debug Function
*/


void DebugBuildingScore(Building* building, int score, int crime, int pollution, int localInfluence, int populationEffect, int randomEffect)
{
    MicroCity::GetInstance().UpdateBuildingScore(building, score, crime, pollution, localInfluence, populationEffect, randomEffect);
}



/*
    Implements
*/


//
void SaveCity()
{
    auto fs = gpd->file->open(DATA_FILE_NAME, kFileWrite);
    bool success = false;
    if (fs)
    {
        if (gpd->file->write(fs, &State, STATE_SIZE) == STATE_SIZE)
        {
            success = true;
        }
        gpd->file->close(fs);
    }

    if (!success)
    {
        gpd->system->logToConsole("SaveCity failed.");
    }
}


///
bool LoadCity()
{
    auto fs = gpd->file->open(DATA_FILE_NAME, kFileReadData);
    if (!fs)
    {
        return false;
    }

    if (gpd->file->read(fs, &State, STATE_SIZE) != STATE_SIZE)
    {
        gpd->file->close(fs);
        return false;
    }
    gpd->file->close(fs);

    if (State.timeToNextDisaster > MAX_TIME_BETWEEN_DISASTERS)
    {
        State.timeToNextDisaster = MIN_TIME_BETWEEN_DISASTERS;
    }
    return true;
}


///
void PutPixel(uint8_t x, uint8_t y, uint8_t color)
{
    MicroCity::GetInstance().PutPixel(x, y, color);
}


///
void DrawBitmap(const uint8_t* data, uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
    MicroCity::GetInstance().DrawBitmap(data, x, y, w, h);
}


///
uint8_t GetInput()
{
    static PDButtons btns;
    gpd->system->getButtonState(&btns, NULL, NULL);
    return btns;
}


///
uint8_t* GetPowerGrid()
{
    static uint8_t PowerGrid[DISPLAY_WIDTH * DISPLAY_HEIGHT / 8];
    return PowerGrid;
}

#ifdef USE_CAPI_MENU
///
static void _OnMenuCityInfo(void* ud)
{
    MicroCity::GetInstance().OnMenuCityInfo();
}
#else
///
static int _OnMenuCityInfo_FromLua(lua_State* L)
{
    MicroCity::GetInstance().OnMenuCityInfo();
    return 0;
}
#endif


///
MicroCity::MicroCity()
: m_bStarted(false)
{
}



/*
    public
*/


///
MicroCity::~MicroCity()
{
    m_spBuildingScore = NULL;
    m_spMenu = NULL;
    m_spDrawDisplay = NULL;
}


///
void MicroCity::Initialize()
{
    m_spBuildingScore = std::make_shared<BuildingScore>();
    m_spMenu = std::make_shared<Menu>();
    m_spMenu->Initialize(m_spBuildingScore);
    m_spDrawDisplay = DrawLCDBitmap::CreateInstance(DISPLAY_WIDTH, DISPLAY_HEIGHT, kColorBlack);
}


///
void MicroCity::Update()
{
    auto info = m_spMenu->GetCityInfo();
    if (info)
    {
        info->Update();
        return;
    }

    static const int POSX = (400 - (DISPLAY_WIDTH * PLAYDATE_ZOOM_SCALE)) / 2;
    static const int POSY = (240 - (DISPLAY_HEIGHT * PLAYDATE_ZOOM_SCALE)) / 2;

    TickGame();

    gpd->graphics->drawScaledBitmap(m_spDrawDisplay->GetLCDBitmap(), POSX, POSY, PLAYDATE_ZOOM_SCALE, PLAYDATE_ZOOM_SCALE);
    //gpd->system->drawFPS(0,0);

    if (!m_bStarted)
    {
        if (UIState.state != StartScreen && UIState.state != NewCityMenu)
        {
            m_bStarted = true;

            #ifdef USE_CAPI_MENU
                gpd->system->addMenuItem("city info", _OnMenuCityInfo, NULL);
            #else
                const char* outerr = NULL;
                gpd->lua->pushString("city info");
                gpd->lua->pushFunction(_OnMenuCityInfo_FromLua);
                gpd->lua->callFunction("_addMenuFromLua", 2, &outerr);
            #endif
        }
    }
}


///
void MicroCity::OnPause()
{
    m_spMenu->OnExitCityInfo();
    auto menuBmp = m_spMenu->GetMenuBitmap();
    if (menuBmp)
    {
        gpd->graphics->drawBitmap(menuBmp, 0, 0, kBitmapUnflipped);
        gpd->system->setMenuImage(menuBmp, 0);
    }
}


///
void MicroCity::OnMenuCityInfo()
{
    m_spMenu->OnMenuCityInfo();
}


///
void MicroCity::OnExitCityInfo()
{
    m_spMenu->OnExitCityInfo();
}


///
void MicroCity::PutPixel(uint8_t x, uint8_t y, uint8_t color)
{
    m_spDrawDisplay->PutPixel(x, y, color);
}


///
void MicroCity::DrawBitmap(const uint8_t* data, uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
    m_spDrawDisplay->DrawBitmap(data, x, y, w, h);
}


///
void MicroCity::UpdateBuildingScore(Building* building, int score, int crime, int pollution, int localInfluence, int populationEffect, int randomEffect)
{
    m_spBuildingScore->UpdateBuildingScore(building, score, crime, pollution, localInfluence, populationEffect, randomEffect);
}


///
MicroCity& MicroCity::GetInstance()
{
    if (m_gspInstance == NULL)
    {
        m_gspInstance = std::shared_ptr<MicroCity>(new MicroCity());
    }
    return *m_gspInstance;
}


///
void MicroCity::Finalize()
{
    m_gspInstance = NULL;
}
