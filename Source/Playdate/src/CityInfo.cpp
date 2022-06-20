#include "CityInfo.h"
#include "MicroCity.h"


static const int MAP_BITMAP_WIDTH = 200;
static const int MAP_BITMAP_HEIGHT = 240;

static const int MAP_TEXT_POSY = 16;

static const int MAP_POSX = 28;
static const int MAP_POSY = 48;



///
void CityInfo::DrawContextCurrentMap()
{
    gpd->graphics->setFont(m_opFontTitle);
    for (int i = 0; i < MAP_INFO_COUNT; i++)
    {
        auto bmp = gpd->graphics->newBitmap(MAP_BITMAP_WIDTH, MAP_BITMAP_HEIGHT, kColorWhite);
        gpd->graphics->pushContext(bmp);
        
        auto width = gpd->graphics->getTextWidth(m_opFontTitle, MAP_DESCS[i], strlen(MAP_DESCS[i]), kASCIIEncoding, 0);
        gpd->graphics->drawText(MAP_DESCS[i], strlen(MAP_DESCS[i]), kASCIIEncoding,
            ((400 - MAP_BITMAP_WIDTH) / 2) - (width / 2), MAP_TEXT_POSY);

        m_spDrawMap->DrawCurrentMap(MAP_INFOS[i], MAP_POSX, MAP_POSY);

        gpd->graphics->popContext();
        m_opMaps[i] = bmp;
    }
}



static const int OVERVIEW_BITMAP_WIDTH = 200;
static const int OVERVIEW_BITMAP_HEIGHT = 240;

static const int OVERVIEW_TEXT_LEFT = 10;
static const int OVERVIEW_TEXT_TOP = 7;

static const int OVERVIEW_TEXT_XPOS = 100;
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
    gpd->graphics->drawBitmap(m_opMaps[m_nMapSelected], 200, 0, kBitmapUnflipped);
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
CityInfo::CityInfo(const std::shared_ptr<DrawMap>& drawMap)
: m_opFontTitle(NULL)
, m_opFontOverview(NULL)
, m_spDrawMap(drawMap)
, m_opOverview(NULL)
, m_nMapSelected(0)
{
    // TODO cache font
    const char* err = NULL;
    m_opFontTitle = gpd->graphics->loadFont("assets/fonts/font-Cuberick-Bold", &err);
    m_opFontOverview = gpd->graphics->loadFont("assets/fonts/font-Cuberick-Bold", &err);
    m_spCityOverview = std::make_shared<CityOverview>();
    m_spCityOverview->UpdateOverview();
}


///
CityInfo::~CityInfo()
{
    gpd->graphics->freeBitmap(m_opOverview);
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
}


///
void CityInfo::Update()
{
    DrawCityInfo();
    
    static PDButtons pressed, released;
    gpd->system->getButtonState(NULL, &pressed, &released);
    if ((pressed & kButtonUp) == kButtonUp)
    {
        m_nMapSelected--;
        if (m_nMapSelected < 0)
        {
            m_nMapSelected = 0;
        }
    }
    else if ((pressed & kButtonDown) == kButtonDown)
    {
        m_nMapSelected++;
        if (m_nMapSelected > MAP_INFO_COUNT - 1)
        {
            m_nMapSelected = MAP_INFO_COUNT - 1;
        }
    }
    else if ((released & kButtonB) == kButtonB)
    {
        MicroCity::GetInstance().OnExitCityInfo();
    }
}



