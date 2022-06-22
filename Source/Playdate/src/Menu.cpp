#include "Menu.h"
#include "Interface.h"



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
    m_spCityInfo = std::make_shared<CityInfo>(m_spDrawMap);

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
    assert(m_spCityInfo);
    m_spCityInfo->InitializeCityInfo();
    m_spCityInfoActive = m_spCityInfo;
}


///
void Menu::OnExitCityInfo()
{
    m_spCityInfoActive = NULL;
}


///
std::shared_ptr<CityInfo>& Menu::GetCityInfo()
{
    return m_spCityInfoActive;
}



