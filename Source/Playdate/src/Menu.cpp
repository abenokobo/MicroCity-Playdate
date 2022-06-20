#include "Menu.h"
#include "Interface.h"





///
void Menu::DrawMapCursor(int mapLeft, int mapTop)
{
    ///
    static const LCDPattern cursor = {
        0b10101010,
        0b01010101,
        0b10101010,
        0b01010101,
        0b10101010,
        0b01010101,
        0b10101010,
        0b01010101,

        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    };

    int tileScrollX = (UIState.scrollX + 4) >> TILE_SIZE_SHIFT;
    int tileScrollY = (UIState.scrollY + 4) >> TILE_SIZE_SHIFT;

    for (int i = 0; i < PLAYDATE_ZOOM_SCALE; i++)
    {
        gpd->graphics->drawRect(
            mapLeft + (tileScrollX * PLAYDATE_ZOOM_SCALE) + i,
            mapTop + (tileScrollY * PLAYDATE_ZOOM_SCALE) + i,
            (DISPLAY_WIDTH / TILE_SIZE) * PLAYDATE_ZOOM_SCALE - (i * 2),
            (DISPLAY_HEIGHT / TILE_SIZE) * PLAYDATE_ZOOM_SCALE - (i * 2),
            (LCDColor)cursor);
    }
}


///
Menu::Menu()
: m_bmpMenu(NULL)
{
}


///
Menu::~Menu()
{
    if (m_bmpMenu)
    {
        gpd->graphics->freeBitmap(m_bmpMenu);
    }
}

///
void Menu::Initialize(const std::shared_ptr<BuildingScore>& buildingScore)
{
    m_spDrawMap = std::make_shared<DrawMap>();
    m_spDrawMap->Initialize(buildingScore);

    m_bmpMenu = gpd->graphics->newBitmap(400, 240, kColorWhite);
}



///
LCDBitmap* Menu::GetMenuBitmap()
{
    static const int MAP_POSX = 28;
    static const int MAP_POSY = 48;

    if (UIState.state != StartScreen && UIState.state != NewCityMenu)
    {
        gpd->graphics->pushContext(m_bmpMenu);
        gpd->graphics->clear(kColorWhite);

        m_spDrawMap->DrawCurrentMap(MapInfo_None, MAP_POSX, MAP_POSY);

        gpd->graphics->popContext();
        
        return m_bmpMenu;
    }

    return NULL;
}


///
void Menu::OnMenuCityInfo()
{
    m_spCityInfo = std::make_shared<CityInfo>(m_spDrawMap);
    m_spCityInfo->Initialize();
    assert(m_spCityInfo);
}


///
void Menu::OnExitCityInfo()
{
    m_spCityInfo = NULL;
}


///
std::shared_ptr<CityInfo>& Menu::GetCityInfo()
{
    return m_spCityInfo;
}



