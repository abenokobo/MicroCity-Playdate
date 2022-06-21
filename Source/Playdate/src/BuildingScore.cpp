#include "BuildingScore.h"
#include "Dithering.h"


static const int BUILDING_ELEMENT_SIZE = 3;



///
void BuildingScore::ClearBuildingScore()
{
    gpd->graphics->pushContext(m_bmpWork);
    gpd->graphics->clear(kColorBlack);
    gpd->graphics->popContext();

    gpd->graphics->pushContext(m_bmpBuildingScore);
    gpd->graphics->clear(kColorClear);
    gpd->graphics->popContext();

    ::memset(&m_grayBuildingScore, 0, sizeof(uint8_t) * BS_WIDTH * BS_HEIGHT);
}


///
void BuildingScore::PutBuildingScore(uint8_t x, uint8_t y, uint8_t score)
{
    int nX = x * PLAYDATE_ZOOM_SCALE;
    int nY = y * PLAYDATE_ZOOM_SCALE;
    for (int i = 0; i < PLAYDATE_ZOOM_SCALE * BUILDING_ELEMENT_SIZE; i++)
    {
        for (int j = 0; j < PLAYDATE_ZOOM_SCALE * BUILDING_ELEMENT_SIZE; j++)
        {
            m_grayBuildingScore[nX + i + ((nY + j) * BS_WIDTH)] = score;
        }
    }
}


///
void BuildingScore::UpdateBuildingScore(BuildingScoreKind kind)
{
    ClearBuildingScore();
    for (int n = 0; n < MAX_BUILDINGS; n++)
    {
        PutBuildingScore(State.buildings[n].x, State.buildings[n].y, m_buildingScore[n][kind]);
    }
}



///
BuildingScore::BuildingScore()
: m_bmpWork(NULL)
, m_bmpBuildingScore(NULL)
{
    ::memset(&m_buildingScore, 0, sizeof(int) * MAX_BUILDINGS * BuildingScoreKind_COUNT);
}


///
BuildingScore::~BuildingScore()
{
    if (m_bmpWork)
    {
        gpd->graphics->freeBitmap(m_bmpWork);
    }
    
    if (m_bmpBuildingScore)
    {
        gpd->graphics->freeBitmap(m_bmpBuildingScore);
    }
}



// from Simulation.cpp
static const float SIM_MAX_CRIME = 50.0f;


///
void BuildingScore::UpdateBuildingScore(Building* building, int score, int crime, int pollution, int localInfluence, int populationEffect, int randomEffect)
{
    for (int n = 0; n < MAX_BUILDINGS; n++)
    {
        if (building == &State.buildings[n])
        {
            int populationDensity = (building->populationDensity + 1) << 4;
            m_buildingScore[n][BuildingScoreKind_PopulationDestiny] = populationDensity < 0xff ? populationDensity : 0xff;

            int recCrime = static_cast<int>((crime / SIM_MAX_CRIME) * 0xff);
            m_buildingScore[n][BuildingScoreKind_Crime] = recCrime < 0xff ? recCrime : 0xff;

            m_buildingScore[n][BuildingScoreKind_Pollution] = pollution < 0xff ? pollution : 0xff;
            return;
        }
    }
}


///
void BuildingScore::DrawBuildingScore(BuildingScoreKind kind, uint8_t x, uint8_t y)
{
    if (m_bmpBuildingScore == NULL)
    {
        m_bmpWork = gpd->graphics->newBitmap(MAP_WIDTH * PLAYDATE_ZOOM_SCALE, MAP_HEIGHT * PLAYDATE_ZOOM_SCALE, kColorBlack);
        m_bmpBuildingScore = gpd->graphics->newBitmap(MAP_WIDTH * PLAYDATE_ZOOM_SCALE, MAP_HEIGHT * PLAYDATE_ZOOM_SCALE, kColorClear);
    }
    assert(m_bmpWork != NULL);
    assert(m_bmpBuildingScore != NULL);

    UpdateBuildingScore(kind);

    int dummy;
    uint8_t* pdummy;
    int rowBytes = 0;
    uint8_t* dstBuf = NULL;
    gpd->graphics->getBitmapData(m_bmpWork, &dummy, &dummy, &rowBytes, &pdummy, &dstBuf);
    Dithering::FloydSteinberg(m_grayBuildingScore, BS_WIDTH, BS_HEIGHT, dstBuf, rowBytes, false);

    gpd->graphics->pushContext(m_bmpBuildingScore);
    gpd->graphics->setDrawMode(kDrawModeInverted);
    gpd->graphics->drawBitmap(m_bmpWork, 0, 0, kBitmapUnflipped);
    gpd->graphics->popContext();

    gpd->graphics->setDrawMode(kDrawModeWhiteTransparent);
    gpd->graphics->drawBitmap(m_bmpBuildingScore, x, y, kBitmapUnflipped);
    gpd->graphics->setDrawMode(kDrawModeCopy);

#if 0
    gpd->lua->pushBitmap(m_bmpBuildingScore);
    gpd->lua->pushString("debug.gif");
    const char* outerr = NULL;
    gpd->lua->callFunction("playdate.datastore.writeImage", 2, &outerr);
#endif
}



