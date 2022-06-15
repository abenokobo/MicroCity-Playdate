#include "MicroCity.h"
#include "Game.h"
#include "DrawMap.h"


///
static const char* DATA_FILE_NAME = "city.dat";

///
static const int STATE_SIZE = sizeof(GameState);


///
MicroCity MicroCity::m_goInstance;

///
static std::shared_ptr<DrawLCDBitmap> m_gspDraw;



/*
	Debug Function
*/


void DebugBuildingScore(Building* building, int score, int crime, int pollution, int localInfluence, int populationEffect, int randomEffect)
{
	// for (int n = 0; n < MAX_BUILDINGS; n++)
	// {
	// 	if (building == &State.buildings[n])
	// 	{
	// 		BuildingDebugValues[n].score = score;
	// 		BuildingDebugValues[n].crime = crime;
	// 		BuildingDebugValues[n].pollution = pollution;
	// 		BuildingDebugValues[n].localInfluence = localInfluence;
	// 		BuildingDebugValues[n].populationEffect = populationEffect;
	// 		BuildingDebugValues[n].randomEffect = randomEffect;
	// 		return;
	// 	}
	// }
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
	m_gspDraw->PutPixel(x, y, color);
}


///
void DrawBitmap(const uint8_t* data, uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
	m_gspDraw->DrawBitmap(data, x, y, w, h);
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


///
MicroCity::MicroCity()
{
}



/*
	public
*/


///
void MicroCity::Initialize()
{
	m_spDraw = DrawLCDBitmap::CreateInstance(DISPLAY_WIDTH, DISPLAY_HEIGHT, kColorBlack);

	m_gspDraw = m_spDraw;

	DrawMap::GetInstance().Initialize();
}


///
void MicroCity::Update()
{
	static const int POSX = (400 - (DISPLAY_WIDTH * PLAYDATE_ZOOM_SCALE)) / 2;
	static const int POSY = (240 - (DISPLAY_HEIGHT * PLAYDATE_ZOOM_SCALE)) / 2;

	TickGame();

	gpd->graphics->drawScaledBitmap(m_gspDraw->GetLCDBitmap(), POSX, POSY, PLAYDATE_ZOOM_SCALE, PLAYDATE_ZOOM_SCALE);
	//gpd->system->drawFPS(0,0);
}


///
LCDBitmap* MicroCity::GetMenuBitmap()
{
	return DrawMap::GetInstance().GetMenuBitmap();
}


///
MicroCity& MicroCity::GetInstance()
{
	return m_goInstance;
}


