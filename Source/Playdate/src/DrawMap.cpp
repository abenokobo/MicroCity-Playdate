#include "DrawMap.h"
#include "Game.h"
#include "Interface.h"
#include "DrawLCDBitmap.h"




///
void DrawMap::CreateTerrainBitmap(uint8_t terrainType)
{
    m_bmpTerrains[terrainType] = DrawLCDBitmap::CreateInstance(MAP_WIDTH, MAP_HEIGHT, kColorBlack);
    m_bmpTerrains[terrainType]->DrawBitmap(GetTerrainData(terrainType), 0, 0, MAP_WIDTH, MAP_HEIGHT);
}


///
void DrawMap::CreateTerrainBitmaps()
{
    for (uint8_t i = 0; i < MAP_COUNT; i++)
    {
        CreateTerrainBitmap(i);
    }
}


///
void DrawMap::DrawMapFrame(int mapLeft, int mapTop)
{
    static const int MAP_BORDER = 3;
    static const int MAP_MARGIN = 3;

    gpd->graphics->fillRect(
        mapLeft - MAP_BORDER - MAP_MARGIN,
        mapTop - MAP_BORDER - MAP_MARGIN,
        (MAP_WIDTH * PLAYDATE_ZOOM_SCALE) + ((MAP_BORDER + MAP_MARGIN) * 2),
        (MAP_HEIGHT * PLAYDATE_ZOOM_SCALE) + ((MAP_BORDER + MAP_MARGIN) * 2),
        kColorBlack);
    
    gpd->graphics->fillRect(
        mapLeft - MAP_MARGIN,
        mapTop - MAP_MARGIN,
        (MAP_WIDTH * PLAYDATE_ZOOM_SCALE) + (MAP_MARGIN * 2),
        (MAP_HEIGHT * PLAYDATE_ZOOM_SCALE) + (MAP_MARGIN * 2),
        kColorWhite);
}


///
void DrawMap::DrawCurrentTerrain(int mapLeft, int mapTop)
{
    gpd->graphics->drawScaledBitmap(
        m_bmpTerrains[State.terrainType]->GetLCDBitmap(),
        mapLeft, mapTop, PLAYDATE_ZOOM_SCALE, PLAYDATE_ZOOM_SCALE);
}


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


///
void DrawMap::DrawMapCursor(int mapLeft, int mapTop)
{
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
DrawMap::DrawMap()
{
}


///
DrawMap::~DrawMap()
{
}


///
bool DrawMap::Initialize(const std::shared_ptr<BuildingScore>& buildingScore)
{
    m_spBuildingScore = buildingScore;
    CreateTerrainBitmaps();
    return true;
}


///
void DrawMap::DrawCurrentMap(MapInfo info, int mapLeft, int mapTop)
{
    DrawMapFrame(mapLeft, mapTop);
    DrawCurrentTerrain(mapLeft, mapTop);

    switch (info)
    {
    case MapInfo_PopulationDestiny:
        m_spBuildingScore->DrawBuildingScore(BuildingScoreKind_PopulationDestiny, mapLeft, mapTop);
        break;

    case MapInfo_Crime:
        m_spBuildingScore->DrawBuildingScore(BuildingScoreKind_Crime, mapLeft, mapTop);
        break;

    case MapInfo_Pollution:
        m_spBuildingScore->DrawBuildingScore(BuildingScoreKind_Pollution, mapLeft, mapTop);
        break;

    case MapInfo_None:
    default:
        break;
    }

    DrawMapCursor(mapLeft, mapTop);
}







