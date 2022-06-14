#include "MicroCity.h"
#include "Interface.h"
#include "Game.h"


//
static const char* DATA_FILE_NAME = "city.dat";


//
static LCDBitmap* m_bmp = NULL;
static uint8_t* m_data = NULL;
static const int STATE_SIZE = sizeof(GameState);


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
	uint8_t* block = m_data + ((y * DISPLAY_WIDTH) / 8) + (x / 8);
	uint8_t data = 0x80 >> (x % 8);
	*block = color ? *block | data : *block & ~data;
}


///
void DrawBitmap(const uint8_t* data, uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			int blockX = i / 8;
			int blockY = j / 8;
			int blocksPerWidth = w / 8;
			int blockIndex = blockY * blocksPerWidth + blockX;
			uint8_t pixels = data[blockIndex * 8 + i % 8];
			uint8_t mask = 1 << (j % 8);
			if (pixels & mask)
			{
				PutPixel(x + i, y + j, 1);
			}
		}
	}
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



/*
	public
*/


///
void InitializeMicroCity()
{
	m_bmp = gpd->graphics->newBitmap(DISPLAY_WIDTH, DISPLAY_HEIGHT, kColorBlack);

	int dummy;
	gpd->graphics->getBitmapData(m_bmp, &dummy, &dummy, &dummy, &dummy, &m_data);
}


///
void UpdateMicroCity()
{
	static const int POSX = (400 - (DISPLAY_WIDTH * PLAYDATE_ZOOM_SCALE)) / 2;
	static const int POSY = (240 - (DISPLAY_HEIGHT * PLAYDATE_ZOOM_SCALE)) / 2;

	TickGame();

	gpd->graphics->drawScaledBitmap(m_bmp, POSX, POSY, PLAYDATE_ZOOM_SCALE, PLAYDATE_ZOOM_SCALE);
	//gpd->system->drawFPS(0,0);
}



