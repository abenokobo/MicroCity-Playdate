

#ifndef __MENU_H
#define __MENU_H


#include "Common.h"
#include "BuildingScore.h"
#include "CityInfo.h"
#include "DrawMap.h"



class Menu
{
private:


    ///
    std::shared_ptr<DrawMap> m_spDrawMap;

    ///
    std::shared_ptr<CityInfo> m_spCityInfo;

    ///
    std::shared_ptr<CityInfo> m_spCityInfoActive;

    ///
    LCDBitmap* m_bmpMenu;



public:


    ///
    Menu();

    ///
    ~Menu();

    ///
    void Initialize(const std::shared_ptr<BuildingScore>& buildingScore);

    ///
    LCDBitmap* GetMenuBitmap();

    ///
    void OnMenuCityInfo();

    ///
    void OnExitCityInfo();

    ///
    std::shared_ptr<CityInfo>& GetCityInfo();
};


#endif // __MENU_H




