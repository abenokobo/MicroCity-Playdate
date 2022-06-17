#ifndef __MAP_BITMAP_H
#define __MAP_BITMAP_H


#include "Common.h"
#include "CityInfo.h"



class DrawLCDBitmap;


enum MapInfo
{
    MapInfo_None,
    MapInfo_CityMap,
    MapInfo_Population,
    MapInfo_Crime,
    MapInfo_Pollution,
};



class DrawMap
{
private:


    ///
    static const int MAP_COUNT = 3;

    ///
    std::shared_ptr<CityInfo> m_spCityInfo;

    ///
    std::shared_ptr<DrawLCDBitmap> m_bmpTerrains[MAP_COUNT];

    ///
    LCDBitmap* m_bmpMap;

    ///
    LCDBitmap* m_bmpMenu;

    ///
    void CreateTerrainBitmap(uint8_t terrainType);

    ///
    void CreateTerrainBitmaps();

    ///
    void DrawMapCursor(int mapLeft, int mapTop);



public:


    ///
    DrawMap(const std::shared_ptr<CityInfo>& cityInfo);

    ///
    ~DrawMap();

    ///
    bool Initialize();

    ///
    LCDBitmap* GetMapBitmap(uint8_t terrainType, MapInfo info);

    ///
    LCDBitmap* GetMenuBitmap();
};


#endif // __MAP_BITMAP_H






