#include "CityInfo.h"
#include "Dithering.h"



///
void CityInfo::ClearCityInfo()
{
	gpd->graphics->pushContext(m_bmpWork);
	gpd->graphics->clear(kColorBlack);
	gpd->graphics->popContext();

	gpd->graphics->pushContext(m_bmpCityInfo);
	gpd->graphics->clear(kColorClear);
	gpd->graphics->popContext();

	::memset(&m_grayCityInfo, 0, sizeof(uint8_t) * INFO_WIDTH * INFO_HEIGHT);
}


///
void CityInfo::PutCityInfo(uint8_t x, uint8_t y, uint8_t score)
{
	int nX = x * PLAYDATE_ZOOM_SCALE;
	int nY = y * PLAYDATE_ZOOM_SCALE;
	for (int i = 0; i < PLAYDATE_ZOOM_SCALE * 3; i++)
	{
		for (int j = 0; j < PLAYDATE_ZOOM_SCALE * 3; j++)
		{
			m_grayCityInfo[nX + i + ((nY + j) * INFO_WIDTH)] = score;
		}
	}
}


///
void CityInfo::UpdateCityInfo(CityInfoKind kind)
{
	ClearCityInfo();
	for (int n = 0; n < MAX_BUILDINGS; n++)
	{
		PutCityInfo(State.buildings[n].x, State.buildings[n].y, m_buildingScore[n][kind]);
	}
}



///
CityInfo::CityInfo()
: m_bmpWork(NULL)
, m_bmpCityInfo(NULL)
{
	::memset(&m_buildingScore, 0, sizeof(int) * MAX_BUILDINGS * CityInfoKind_COUNT);
}


///
void CityInfo::UpdateBuildingScore(Building* building, int score, int crime, int pollution, int localInfluence, int populationEffect, int randomEffect)
{
	for (int n = 0; n < MAX_BUILDINGS; n++)
	{
		if (building == &State.buildings[n])
		{
			m_buildingScore[n][CityInfoKind_Crime] = crime < 0xff ? crime : 0xff;
			m_buildingScore[n][CityInfoKind_Pollution] = pollution < 0xff ? pollution : 0xff;
			return;
		}
	}
}


///
void CityInfo::DrawCityInfo(CityInfoKind kind)
{
	if (m_bmpCityInfo == NULL)
	{
		m_bmpWork = gpd->graphics->newBitmap(MAP_WIDTH * PLAYDATE_ZOOM_SCALE, MAP_HEIGHT * PLAYDATE_ZOOM_SCALE, kColorBlack);
		m_bmpCityInfo = gpd->graphics->newBitmap(MAP_WIDTH * PLAYDATE_ZOOM_SCALE, MAP_HEIGHT * PLAYDATE_ZOOM_SCALE, kColorClear);
	}
	assert(m_bmpWork != NULL);
	assert(m_bmpCityInfo != NULL);

	UpdateCityInfo(kind);

	int dummy;
	uint8_t* pdummy;
	int rowBytes = 0;
	uint8_t* dstBuf = NULL;
	gpd->graphics->getBitmapData(m_bmpWork, &dummy, &dummy, &rowBytes, &pdummy, &dstBuf);
	Dithering::FloydSteinberg(m_grayCityInfo, INFO_WIDTH, INFO_HEIGHT, dstBuf, rowBytes, false);

	gpd->graphics->pushContext(m_bmpCityInfo);
	gpd->graphics->setDrawMode(kDrawModeInverted);
	gpd->graphics->drawBitmap(m_bmpWork, 0, 0, kBitmapUnflipped);
	gpd->graphics->popContext();

	gpd->graphics->setDrawMode(kDrawModeWhiteTransparent);
	gpd->graphics->drawBitmap(m_bmpCityInfo, 0, 0, kBitmapUnflipped);
	gpd->graphics->setDrawMode(kDrawModeCopy);

#if 0
    gpd->lua->pushBitmap(m_bmpCityInfo);
    gpd->lua->pushString("debug.gif");
    const char* outerr = NULL;
    gpd->lua->callFunction("playdate.datastore.writeImage", 2, &outerr);
#endif
}







