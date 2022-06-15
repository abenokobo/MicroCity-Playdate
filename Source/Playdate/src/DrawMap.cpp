#include "DrawMap.h"
#include "Game.h"
#include "Interface.h"
#include "DrawLCDBitmap.h"



DrawMap DrawMap::m_goInstance;


///
DrawMap::DrawMap()
: m_bmpMenu(NULL)
{
}


///
DrawMap::~DrawMap()
{
	if (m_bmpMenu)
	{
		gpd->graphics->freeBitmap(m_bmpMenu);
	}
}



///
void DrawMap::CreateCityDrawMap(uint8_t terrainType)
{
    m_bmpMaps[terrainType] = DrawLCDBitmap::CreateInstance(MAP_WIDTH, MAP_HEIGHT, kColorBlack);
    m_bmpMaps[terrainType]->DrawBitmap(GetTerrainData(terrainType), 0, 0, MAP_WIDTH, MAP_HEIGHT);
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
bool DrawMap::Initialize()
{
    m_bmpMenu = gpd->graphics->newBitmap(400, 240, kColorWhite);
    if (!m_bmpMenu)
    {
        assert(false);
        return false;
    }

    for (uint8_t i = 0; i < MAP_COUNT; i++)
    {
        CreateCityDrawMap(i);
    }
    return true;
}


///
LCDBitmap* DrawMap::GetLCDBitmap(uint8_t terrainType)
{
    return m_bmpMaps[terrainType]->GetLCDBitmap();
}



static const int MAP_POSX = 28;
static const int MAP_POSY = 48;
static const int MAP_BORDER = 3;
static const int MAP_MARGIN = 3;



///
LCDBitmap* DrawMap::GetMenuBitmap()
{
	if (UIState.state != StartScreen && UIState.state != NewCityMenu)
	{
		gpd->graphics->pushContext(m_bmpMenu);
		gpd->graphics->clear(kColorWhite);

		gpd->graphics->fillRect(
			MAP_POSX - MAP_BORDER - MAP_MARGIN,
			MAP_POSY - MAP_BORDER - MAP_MARGIN,
			(MAP_WIDTH * PLAYDATE_ZOOM_SCALE) + ((MAP_BORDER + MAP_MARGIN) * 2),
			(MAP_HEIGHT * PLAYDATE_ZOOM_SCALE) + ((MAP_BORDER + MAP_MARGIN) * 2),
			kColorBlack);
		
		gpd->graphics->fillRect(
			MAP_POSX - MAP_MARGIN,
			MAP_POSY - MAP_MARGIN,
			(MAP_WIDTH * PLAYDATE_ZOOM_SCALE) + (MAP_MARGIN * 2),
			(MAP_HEIGHT * PLAYDATE_ZOOM_SCALE) + (MAP_MARGIN * 2),
			kColorWhite);

		gpd->graphics->drawScaledBitmap(
			DrawMap::GetInstance().GetLCDBitmap(State.terrainType),
			MAP_POSX, MAP_POSY,
			PLAYDATE_ZOOM_SCALE, PLAYDATE_ZOOM_SCALE);

        DrawMapCursor(MAP_POSX, MAP_POSY);

		gpd->graphics->popContext();
		return m_bmpMenu;
	}
	return NULL;
}



///
DrawMap& DrawMap::GetInstance()
{
    return m_goInstance;
}



