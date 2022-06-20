#ifndef __CITY_INFO_H
#define __CITY_INFO_H


#include "Common.h"
#include "DrawMap.h"
#include "CityOverview.h"



class CityInfo
{
private:


    ///
    LCDFont* m_opFontTitle;

    ///
    LCDFont* m_opFontOverview;

    ///
    std::shared_ptr<DrawMap> m_spDrawMap;

    ///
    std::shared_ptr<CityOverview> m_spCityOverview;


    ///
    static const int MAP_INFO_COUNT = 3;
    static const MapInfo MAP_INFOS[MAP_INFO_COUNT];
    static const char* MAP_DESCS[MAP_INFO_COUNT];

    ///
    LCDBitmap* m_opOverview;

    ///
    LCDBitmap* m_opMaps[MAP_INFO_COUNT];

    ///
    int m_nMapSelected;

    ///
    void DrawContextCurrentMap();

    ///
    void DrawOverviewText(const char* szTitle, const char* szDsc, bool breakY, int& x, int& y);

    ///
    void DrawContextCityOverview();

    ///
    void DrawCurrentMap();

    ///
    void DrawCityOverview();

    ///
    void DrawCityInfo();



public:


    ///
    CityInfo(const std::shared_ptr<DrawMap>& drawMap);

    ///
    ~CityInfo();

    ///
    void Initialize();

    ///
    void Update();
};


#endif // __CITY_INFO_H



