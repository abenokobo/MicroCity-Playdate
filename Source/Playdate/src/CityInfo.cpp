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
    const char* err = NULL;
    auto bmpFrame = gpd->graphics->loadBitmap("assets/images/frame", &err);

    gpd->graphics->setFont(m_opFontTitle);
    for (int i = 0; i < MAP_INFO_COUNT; i++)
    {
        auto bmp = gpd->graphics->newBitmap(MAP_BITMAP_WIDTH, MAP_BITMAP_HEIGHT, kColorWhite);
        gpd->graphics->pushContext(bmp);
        gpd->graphics->drawBitmap(bmpFrame, 0, 0, kBitmapUnflipped);
        
        const char* text = MAP_DESCS[i];
        size_t len = strlen(text);
        auto width = gpd->graphics->getTextWidth(m_opFontTitle, text, len, kASCIIEncoding, 0);
        int x = (MAP_BITMAP_WIDTH / 2) - (width / 2);
        
        gpd->graphics->fillRect(
            x - MAP_TEXT_MARGINX, MAP_TEXT_POSY,
            width + (MAP_TEXT_MARGINX * 2), 14, kColorWhite);

        gpd->graphics->drawText(text, len, kASCIIEncoding,
            x, MAP_TEXT_POSY);

        m_spDrawMap->DrawCurrentMap(MAP_INFOS[i], MAP_POSX, MAP_POSY);

        gpd->graphics->popContext();
        m_opMaps[i] = bmp;
    }

    gpd->graphics->freeBitmap(bmpFrame);
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
    auto width = gpd->graphics->getTextWidth(m_opFontTitle, szTitle, strlen(szTitle), kASCIIEncoding, 0);
    gpd->graphics->drawText(szTitle, strlen(szTitle), kASCIIEncoding, x + OVERVIEW_TEXT_XPOS - width, y);
    gpd->graphics->drawText(szDsc, strlen(szDsc), kASCIIEncoding, x + OVERVIEW_TEXT_XPOS + OVERVIEW_TEXT_MARGIN_WIDTH, y);

    y += OVERVIEW_TEXT_HEIGHT;
    if (breakY)
    {
        y += OVERVIEW_BREAK_HEIGHT;
    }
}


///
void CityInfo::DrawContextCityOverview()
{
    m_opOverview = gpd->graphics->newBitmap(OVERVIEW_BITMAP_WIDTH, OVERVIEW_BITMAP_HEIGHT, kColorWhite);
    gpd->graphics->pushContext(m_opOverview);

    gpd->graphics->setFont(m_opFontOverview);

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
    DrawOverviewText("Population :", szBuf, true, x, y);


    sprintf(szBuf, "%d (%d%%)", m_spCityOverview->nResidential, nPerResidential);
    DrawOverviewText("Residential :", szBuf, false, x, y);

    sprintf(szBuf, "%d (%d%%)", m_spCityOverview->nCommercial, nPerCommercial);
    DrawOverviewText("Commercial :", szBuf, false, x, y);

    sprintf(szBuf, "%d (%d%%)", m_spCityOverview->nIndustrial, nPerIndustrial);
    DrawOverviewText("Industrial :", szBuf, false, x, y);

    sprintf(szBuf, "%d (%d%%)", nOther, nPerOther);
    DrawOverviewText("Other :", szBuf, false, x, y);

    sprintf(szBuf, "%d", nTotal);
    DrawOverviewText("Total :", szBuf, true, x, y);


    sprintf(szBuf, "%d", m_spCityOverview->nPark);
    DrawOverviewText("Park :", szBuf, false, x, y);

    sprintf(szBuf, "%d", m_spCityOverview->nPoliceDept);
    DrawOverviewText("PoliceDept :", szBuf, false, x, y);

    sprintf(szBuf, "%d", m_spCityOverview->nFireDept);
    DrawOverviewText("FireDept :", szBuf, false, x, y);

    sprintf(szBuf, "%d", m_spCityOverview->nStadium);
    DrawOverviewText("Stadium :", szBuf, false, x, y);

    sprintf(szBuf, "%d", m_spCityOverview->nPowerplant);
    DrawOverviewText("Powerplant :", szBuf, true, x, y);


    sprintf(szBuf, "%d", m_spCityOverview->nRoads);
    DrawOverviewText("Roads :", szBuf, false, x, y);

    sprintf(szBuf, "%d", m_spCityOverview->nPowerline);
    DrawOverviewText("Powerline :", szBuf, false, x, y);

    gpd->graphics->popContext();
}


///
void CityInfo::DrawCurrentMap()
{
    if (!m_opMapCurrent)
    {
        assert(false);
        return;
    }

    int posY = GetCurrentMapPosY();
    gpd->graphics->drawBitmap(m_opMapCurrent, OVERVIEW_BITMAP_WIDTH, posY, kBitmapUnflipped);

    if (m_opMapLast)
    {
        int prePosY = posY - 240;
        if (posY < 0)
        {
            prePosY =posY + 240;
        }
        gpd->graphics->drawBitmap(m_opMapLast, OVERVIEW_BITMAP_WIDTH, prePosY, kBitmapUnflipped);
    }

    DrawMapCursor();
}


///
void CityInfo::DrawCityOverview()
{
    gpd->graphics->drawBitmap(m_opOverview, 0, 0, kBitmapUnflipped);
}


///
void CityInfo::DrawCityInfo()
{
    gpd->graphics->clear(kColorWhite);
    DrawCurrentMap();
    DrawCityOverview();
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
    if (m_opMapCurrent)
    {
        m_opMapLast = m_opMapCurrent;
    }
    m_opMapCurrent = m_opMaps[index];

    if (m_opMapLast)
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
    if (!m_opMapLast)
    {
        return 0;
    }

    auto time = gpd->system->getCurrentTimeMilliseconds() - m_nCurrentTime;
    if (time > SCROLL_TIME)
    {
        m_opMapLast = NULL;
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
    if (m_opMapLast)
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
        gpd->graphics->drawBitmap(m_opMapCursor, MAP_CURSOR_X, MAP_CURSOR_UP_Y, kBitmapUnflipped);
    }

    // next
    if (m_nMapSelected != MAP_INFO_COUNT - 1)
    {
        gpd->graphics->drawBitmap(m_opMapCursor, MAP_CURSOR_X, MAP_CURSOR_DOWN_Y, kBitmapFlippedY);
    }
}



///
CityInfo::CityInfo(const std::shared_ptr<DrawMap>& drawMap)
: m_opFontTitle(NULL)
, m_opFontOverview(NULL)
, m_spDrawMap(drawMap)
, m_opOverview(NULL)
, m_nMapSelected(0)
, m_opMapCurrent(NULL)
, m_opMapLast(NULL)
, m_nStartPosY(0)
, m_nEndPosY(0)
, m_nCurrentTime(0)
, m_opMapCursor(NULL)
{
    const char* err = NULL;
    m_opFontTitle = gpd->graphics->loadFont("assets/fonts/font-Cuberick-Bold", &err);
    m_opFontOverview = gpd->graphics->loadFont("assets/fonts/font-Cuberick-Bold", &err);
    m_spCityOverview = std::make_shared<CityOverview>();
    m_spCityOverview->UpdateOverview();
    m_opMapCursor = gpd->graphics->loadBitmap("assets/images/cursor", &err);
}


///
CityInfo::~CityInfo()
{
    gpd->graphics->freeBitmap(m_opOverview);
    gpd->graphics->freeBitmap(m_opMapCursor);
    for (int i = 0; i < MAP_INFO_COUNT; i++)
    {
        gpd->graphics->freeBitmap(m_opMaps[i]);
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
void CityInfo::Initialize()
{
    DrawContextCurrentMap();
    DrawContextCityOverview();
    UpdateSelectedMap(0);
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



