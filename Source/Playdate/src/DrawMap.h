#ifndef __DRAW_MAP_H
#define __DRAW_MAP_H


#include "Common.h"
#include "BuildingScore.h"



class DrawLCDBitmap;


enum MapInfo
{
    MapInfo_None,
    MapInfo_PopulationDestiny,
    MapInfo_Crime,
    MapInfo_Pollution,
    MapInfo_COUNT
};



class DrawMap
{
private:


    ///
    static const int MAP_COUNT = 3;

    ///
    std::shared_ptr<BuildingScore> m_spBuildingScore;

    ///
    std::shared_ptr<DrawLCDBitmap> m_bmpTerrains[MAP_COUNT];




    ///
    void CreateTerrainBitmap(uint8_t terrainType);

    ///
    void CreateTerrainBitmaps();

    ///
    void DrawMapFrame(int mapLeft, int mapTop);

    ///
    void DrawCurrentTerrain(int mapLeft, int mapTop);
    
    ///
    void DrawMapCursor(int mapLeft, int mapTop);



public:


    ///
    DrawMap();

    ///
    ~DrawMap();

    ///
    bool Initialize(const std::shared_ptr<BuildingScore>& buildingScore);

    ///
    void DrawCurrentMap(MapInfo info, int mapLeft, int mapTop);
};



#endif // __DRAW_MAP_H






