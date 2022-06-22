#include "CityInfo.h"
#include "MicroCity.h"


static const int MAP_BITMAP_WIDTH = 220;
static const int MAP_BITMAP_HEIGHT = 240;

static const int MAP_TEXT_POSY = 20;
static const int MAP_TEXT_MARGINX = 6;

static const int MAP_POSX = 38;
static const int MAP_POSY = 59;



///
void CityInfo::DrawContextCurrentMap()
{
    gpd->graphics->setFont(m_fntTitle);
    for (int i = 0; i < MAP_INFO_COUNT; i++)
    {
        auto bmp = m_bmpMaps[i];
        gpd->graphics->pushContext(bmp);
        gpd->graphics->drawBitmap(m_bmpFrame, 0, 0, kBitmapUnflipped);
        
        const char* text = MAP_DESCS[i];
        size_t len = strlen(text);
        auto width = gpd->graphics->getTextWidth(m_fntTitle, text, len, kASCIIEncoding, 0);
        int x = (MAP_BITMAP_WIDTH / 2) - (width / 2);
        
        gpd->graphics->fillRect(
            x - MAP_TEXT_MARGINX, MAP_TEXT_POSY,
            width + (MAP_TEXT_MARGINX * 2), 14, kColorWhite);

        gpd->graphics->drawText(text, len, kASCIIEncoding,
            x, MAP_TEXT_POSY);

        m_spDrawMap->DrawCurrentMap(MAP_INFOS[i], MAP_POSX, MAP_POSY);

        gpd->graphics->popContext();
    }
}



static const int OVERVIEW_BITMAP_WIDTH = 400 - MAP_BITMAP_WIDTH;
static const int OVERVIEW_BITMAP_HEIGHT = 240;

static const int OVERVIEW_TEXT_LEFT = 10;
static const int OVERVIEW_TEXT_TOP = 7;

static const int OVERVIEW_TEXT_XPOS = OVERVIEW_BITMAP_WIDTH / 2;
static const int OVERVIEW_TEXT_MARGIN_WIDTH = 10;
static const int OVERVIEW_TEXT_HEIGHT = 16;
static const int OVERVIEW_BREAK_HEIGHT = 7;


///
void CityInfo::DrawOverviewText(const char* szTitle, const char* szDsc, bool breakY, int& x, int& y)
{
    auto width = gpd->graphics->getTextWidth(m_fntTitle, szTitle, strlen(szTitle), kASCIIEncoding, 0);
    gpd->graphics->drawText(szTitle, strlen(szTitle), kASCIIEncoding, x + OVERVIEW_TEXT_XPOS - width, y);
    gpd->graphics->drawText(szDsc, strlen(szDsc), kASCIIEncoding, x + OVERVIEW_TEXT_XPOS + OVERVIEW_TEXT_MARGIN_WIDTH, y);

    y += OVERVIEW_TEXT_HEIGHT;
    if (breakY)
    {
        y += OVERVIEW_BREAK_HEIGHT;
    }
}



///
static const char* TTL_Population   = "Population :";
static const char* TTL_Residential  = "Residential :";
static const char* TTL_Commercial   = "Commercial :";
static const char* TTL_Industrial   = "Industrial :";
static const char* TTL_Other        = "Other :";
static const char* TTL_Total        = "Total :";
static const char* TTL_Park         = "Park :";
static const char* TTL_PoliceDept   = "PoliceDept :";
static const char* TTL_FireDept     = "FireDept :";
static const char* TTL_Stadium      = "Stadium :";
static const char* TTL_Powerplant   = "Powerplant :";
static const char* TTL_Roads        = "Roads :";
static const char* TTL_Powerline    = "Powerline :";



///
void CityInfo::DrawContextCityOverview()
{
    gpd->graphics->pushContext(m_bmpOverview);

    gpd->graphics->clear(kColorWhite);
    gpd->graphics->setFont(m_fntOverview);

    static char szBuf[255];
    int x = OVERVIEW_TEXT_LEFT;
    int y = OVERVIEW_TEXT_TOP;

    int nOther =
        m_spCityOverview->nPowerplant +
        m_spCityOverview->nPark +
        m_spCityOverview->nPoliceDept +
        m_spCityOverview->nFireDept +
        m_spCityOverview->nStadium;

    int nTotal =
        m_spCityOverview->nResidential +
        m_spCityOverview->nCommercial +
        m_spCityOverview->nIndustrial +
        nOther;

    int nPerResidential = nTotal == 0 ? 0 : (int)((m_spCityOverview->nResidential / (float)nTotal) * 100.f);
    int nPerCommercial = nTotal == 0 ? 0 : (int)((m_spCityOverview->nCommercial / (float)nTotal) * 100.f);
    int nPerIndustrial = nTotal == 0 ? 0 : (int)((m_spCityOverview->nIndustrial / (float)nTotal) * 100.f);
    int nPerOther = nTotal == 0 ? 0 : (int)((nOther / (float)nTotal) * 100.f);

    sprintf(szBuf, "%d", m_spCityOverview->nPopulation);
    DrawOverviewText(TTL_Population, szBuf, true, x, y);


    sprintf(szBuf, "%d (%d%%)", m_spCityOverview->nResidential, nPerResidential);
    DrawOverviewText(TTL_Residential, szBuf, false, x, y);

    sprintf(szBuf, "%d (%d%%)", m_spCityOverview->nCommercial, nPerCommercial);
    DrawOverviewText(TTL_Commercial, szBuf, false, x, y);

    sprintf(szBuf, "%d (%d%%)", m_spCityOverview->nIndustrial, nPerIndustrial);
    DrawOverviewText(TTL_Industrial, szBuf, false, x, y);

    sprintf(szBuf, "%d (%d%%)", nOther, nPerOther);
    DrawOverviewText(TTL_Other, szBuf, false, x, y);

    sprintf(szBuf, "%d", nTotal);
    DrawOverviewText(TTL_Total, szBuf, true, x, y);


    sprintf(szBuf, "%d", m_spCityOverview->nPark);
    DrawOverviewText(TTL_Park, szBuf, false, x, y);

    sprintf(szBuf, "%d", m_spCityOverview->nPoliceDept);
    DrawOverviewText(TTL_PoliceDept, szBuf, false, x, y);

    sprintf(szBuf, "%d", m_spCityOverview->nFireDept);
    DrawOverviewText(TTL_FireDept, szBuf, false, x, y);

    sprintf(szBuf, "%d", m_spCityOverview->nStadium);
    DrawOverviewText(TTL_Stadium, szBuf, false, x, y);

    sprintf(szBuf, "%d", m_spCityOverview->nPowerplant);
    DrawOverviewText(TTL_Powerplant, szBuf, true, x, y);


    sprintf(szBuf, "%d", m_spCityOverview->nRoads);
    DrawOverviewText(TTL_Roads, szBuf, false, x, y);

    sprintf(szBuf, "%d", m_spCityOverview->nPowerline);
    DrawOverviewText(TTL_Powerline, szBuf, false, x, y);

    gpd->graphics->popContext();
}


///
void CityInfo::DrawCurrentMap()
{
    if (!m_bmpMapCurrent)
    {
        assert(false);
        return;
    }

    int posY = GetCurrentMapPosY();
    gpd->graphics->drawBitmap(m_bmpMapCurrent, OVERVIEW_BITMAP_WIDTH, posY, kBitmapUnflipped);

    if (m_bmpMapLast)
    {
        int prePosY = posY - 240;
        if (posY < 0)
        {
            prePosY =posY + 240;
        }
        gpd->graphics->drawBitmap(m_bmpMapLast, OVERVIEW_BITMAP_WIDTH, prePosY, kBitmapUnflipped);
    }

    DrawMapCursor();
}


///
void CityInfo::DrawCityOverview()
{
    gpd->graphics->drawBitmap(m_bmpOverview, 0, 0, kBitmapUnflipped);
}


///
void CityInfo::DrawCityInfo()
{
    gpd->graphics->clear(kColorWhite);
    if (m_spCityOverview)
    {
        m_spCityOverview->UpdateOverview();
        DrawContextCurrentMap();
        DrawContextCityOverview();
        UpdateSelectedMap(0);
        m_spCityOverview = NULL;
    }

    DrawCurrentMap();
    DrawCityOverview();
}


///
static const LCDPattern bg = {
    0b10101010,
    0b01010101,
    0b10101010,
    0b01010101,
    0b10101010,
    0b01010101,
    0b10101010,
    0b01010101,

    0b01010101,
    0b10101010,
    0b01010101,
    0b10101010,
    0b01010101,
    0b10101010,
    0b01010101,
    0b10101010,
};


///
void CityInfo::DrawNowLoading()
{
    gpd->graphics->clear(kColorWhite);
    gpd->graphics->setFont(m_fntOverview);

    int x = OVERVIEW_TEXT_LEFT;
    int y = OVERVIEW_TEXT_TOP;

    DrawOverviewText(TTL_Population, "-", true, x, y);
    DrawOverviewText(TTL_Residential, "-", false, x, y);
    DrawOverviewText(TTL_Commercial, "-", false, x, y);
    DrawOverviewText(TTL_Industrial, "-", false, x, y);
    DrawOverviewText(TTL_Other, "-", false, x, y);
    DrawOverviewText(TTL_Total, "-", true, x, y);
    DrawOverviewText(TTL_Park, "-", false, x, y);
    DrawOverviewText(TTL_PoliceDept, "-", false, x, y);
    DrawOverviewText(TTL_FireDept, "-", false, x, y);
    DrawOverviewText(TTL_Stadium, "-", false, x, y);
    DrawOverviewText(TTL_Powerplant, "-", true, x, y);
    DrawOverviewText(TTL_Roads, "-", false, x, y);
    DrawOverviewText(TTL_Powerline, "-", false, x, y);

    gpd->graphics->drawBitmap(m_bmpFrame, OVERVIEW_BITMAP_WIDTH, 0, kBitmapUnflipped);
    gpd->graphics->fillRect(0, 0, 400, 240, (LCDColor)bg);
    gpd->graphics->drawBitmap(m_bmpLoading, 176, 96, kBitmapUnflipped);
}


///
void CityInfo::SelectPreviousMap()
{
    auto selected = m_nMapSelected - 1;
    if (selected < 0)
    {
        return;
    }
    UpdateSelectedMap(selected);
}


///
void CityInfo::SelectNextMap()
{
    auto selected = m_nMapSelected + 1;
    if (selected > MAP_INFO_COUNT - 1)
    {
        return;
    }
    UpdateSelectedMap(selected);
}


///
void CityInfo::UpdateSelectedMap(int index)
{
    if (m_bmpMapCurrent)
    {
        m_bmpMapLast = m_bmpMapCurrent;
    }
    m_bmpMapCurrent = m_bmpMaps[index];

    if (m_bmpMapLast)
    {
        m_nCurrentTime = gpd->system->getCurrentTimeMilliseconds();
    }

    if (m_nMapSelected > index)
    {
        // previous
        m_nStartPosY = -240;
        m_nEndPosY = 0;
    }
    else
    {
        // next
        m_nStartPosY = 240;
        m_nEndPosY = 0;
    }

    m_nMapSelected = index;
}


///
static const int SCROLL_TIME  = 500;


///
int CityInfo::GetCurrentMapPosY()
{
    if (!m_bmpMapLast)
    {
        return 0;
    }

    auto time = gpd->system->getCurrentTimeMilliseconds() - m_nCurrentTime;
    if (time > SCROLL_TIME)
    {
        m_bmpMapLast = NULL;
        return 0;
    }

    // easeOutCubic
    int diff = m_nEndPosY - m_nStartPosY;
    float t = (time / static_cast<float>(SCROLL_TIME)) - 1;
    return static_cast<int>(diff * (t * t * t + 1)) + m_nStartPosY;
}



///
static const int MAP_CURSOR_X = 283;
static const int MAP_CURSOR_UP_Y = 4;
static const int MAP_CURSOR_DOWN_Y = 227;


///
void CityInfo::DrawMapCursor()
{
    if (m_bmpMapLast)
    {
        return;
    }

    // if (gpd->system->getCurrentTimeMilliseconds() % 1000 > 500)
    // {
    //     return;
    // }

    // previous
    if (m_nMapSelected != 0)
    {
        gpd->graphics->drawBitmap(m_bmpMapCursor, MAP_CURSOR_X, MAP_CURSOR_UP_Y, kBitmapUnflipped);
    }

    // next
    if (m_nMapSelected != MAP_INFO_COUNT - 1)
    {
        gpd->graphics->drawBitmap(m_bmpMapCursor, MAP_CURSOR_X, MAP_CURSOR_DOWN_Y, kBitmapFlippedY);
    }
}



///
CityInfo::CityInfo(const std::shared_ptr<DrawMap>& drawMap)
: m_fntTitle(NULL)
, m_fntOverview(NULL)
, m_bmpFrame(NULL)
, m_bmpLoading(NULL)
, m_bmpMapCursor(NULL)
, m_spDrawMap(drawMap)
, m_bmpOverview(NULL)
, m_nMapSelected(0)
, m_bmpMapCurrent(NULL)
, m_bmpMapLast(NULL)
, m_nStartPosY(0)
, m_nEndPosY(0)
, m_nCurrentTime(0)
{
    const char* err = NULL;
    m_fntTitle = gpd->graphics->loadFont("assets/fonts/font-Cuberick-Bold", &err);
    m_fntOverview = gpd->graphics->loadFont("assets/fonts/font-Cuberick-Bold", &err);

    m_bmpFrame = gpd->graphics->loadBitmap("assets/images/frame", &err);
    m_bmpLoading = gpd->graphics->loadBitmap("assets/images/loading", &err);
    m_bmpMapCursor = gpd->graphics->loadBitmap("assets/images/cursor", &err);
    m_bmpOverview = gpd->graphics->newBitmap(OVERVIEW_BITMAP_WIDTH, OVERVIEW_BITMAP_HEIGHT, kColorWhite);
    for (int i = 0; i < MAP_INFO_COUNT; i++)
    {
        m_bmpMaps[i] = gpd->graphics->newBitmap(MAP_BITMAP_WIDTH, MAP_BITMAP_HEIGHT, kColorWhite);
    }
}


///
CityInfo::~CityInfo()
{
    gpd->graphics->freeBitmap(m_bmpFrame);
    gpd->graphics->freeBitmap(m_bmpLoading);
    gpd->graphics->freeBitmap(m_bmpMapCursor);
    gpd->graphics->freeBitmap(m_bmpOverview);
    for (int i = 0; i < MAP_INFO_COUNT; i++)
    {
        gpd->graphics->freeBitmap(m_bmpMaps[i]);
    }
}


///
const MapInfo CityInfo::MAP_INFOS[MAP_INFO_COUNT] = {
    MapInfo_PopulationDestiny,
    MapInfo_Crime,
    MapInfo_Pollution,
};


///
const char* CityInfo::MAP_DESCS[MAP_INFO_COUNT] = {
    "POPULATION DESTINY",
    "CRIME",
    "POLLUTION",
};


///
void CityInfo::InitializeCityInfo()
{
    m_bmpMapCurrent = 0;
    m_bmpMapLast = NULL;
    m_nMapSelected = 0;
    m_bmpMapCurrent = NULL;
    m_bmpMapLast = NULL;
    m_nStartPosY = 0;
    m_nEndPosY = 0;
    m_nCurrentTime = 0;

    m_spCityOverview = std::make_shared<CityOverview>();
    DrawNowLoading();
}


///
void CityInfo::Update()
{
    DrawCityInfo();
    
    static PDButtons pressed, released;
    gpd->system->getButtonState(NULL, &pressed, &released);
    if ((pressed & kButtonUp) == kButtonUp)
    {
        SelectPreviousMap();
    }
    else if ((pressed & kButtonDown) == kButtonDown)
    {
        SelectNextMap();
    }
    else if ((released & kButtonB) == kButtonB)
    {
        MicroCity::GetInstance().OnExitCityInfo();
    }
}



