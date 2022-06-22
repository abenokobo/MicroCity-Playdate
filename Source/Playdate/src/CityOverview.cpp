#include "CityOverview.h"



#ifndef NDEBUG
///
void CityOverview::DebugOut()
{
    gpd->system->logToConsole("=====");
    gpd->system->logToConsole("Population %d", nPopulation);
    gpd->system->logToConsole("R:%d C:%d I:%d", nResidential, nCommercial, nIndustrial);
    gpd->system->logToConsole("PoliceDept:%d FireDept:%d Park:%d", nPoliceDept, nFireDept, nPark);
    gpd->system->logToConsole("Powerplant:%d Stadium:%d", nPowerplant, nStadium);
}
#endif


///
bool CityOverview::IsBuildings(int x, int y)
{
    for (int n = 0; n < MAX_BUILDINGS; n++)
    {
        Building* building = &State.buildings[n];
        if (!building->type)
        {
            continue;
        }

        if (x < building->x || y < building->y)
        {
            continue;
        }

        const BuildingInfo* info = GetBuildingInfo(building->type);
        uint8_t width = pgm_read_byte(&info->width);
        uint8_t height = pgm_read_byte(&info->height);

        if (x < building->x + width && y < building->y + height)
        {
            return true;
        }
    }
    return false;
}


///
void CityOverview::UpdateBuildingsOverview()
{
    for (int n = 0; n < MAX_BUILDINGS; n++)
    {
        Building& bld = State.buildings[n];

        switch (bld.type)
        {
        case Residential:
            nResidential++;
            break;

        case Industrial:
            nIndustrial++;
            break;

        case Commercial:
            nCommercial++;
            break;

        case Powerplant:
            nPowerplant++;
            break;
        
        case Park:
            nPark++;
            break;
        
        case PoliceDept:
            nPoliceDept++;
            break;
        
        case FireDept:
            nFireDept++;
            break;
        
        case Stadium:
            nStadium++;
            break;

        default:
            break;
        }
    }
}


///
void CityOverview::UpdatConnectionsOverview()
{
    for (int y = 0; y < MAX_SCROLL_Y; y++)
    {
        for (int x = 0; x < MAX_SCROLL_X; x++)
        {
            if (IsBuildings(x, y))
            {
                continue;
            }

            auto connections = GetConnections(x, y);
            if ((connections & RoadMask) == RoadMask)
            {
                nRoads++;
            }

            if ((connections & PowerlineMask) == PowerlineMask)
            {
                nPowerline++;
            }
        }
    }
}



///
CityOverview::CityOverview()
: nPopulation(0)
, nResidential(0)
, nCommercial(0)
, nIndustrial(0)
, nPowerplant(0)
, nPark(0)
, nPoliceDept(0)
, nFireDept(0)
, nStadium(0)
, nRoads(0)
, nPowerline(0)
{
}


///
void CityOverview::UpdateOverview()
{
    nPopulation = (State.residentialPopulation + State.commercialPopulation + State.residentialPopulation) * POPULATION_MULTIPLIER;
    UpdateBuildingsOverview();
    UpdatConnectionsOverview();

#ifndef NDEBUG
#ifdef _WIN32
    //_sleep(5000);
#endif
    //DebugOut();
#endif
}


