#ifndef __CITY_OVERVIEW_H
#define __CITY_OVERVIEW_H


#include "Common.h"
#include "Game.h"


class CityOverview
{
private:


#ifndef NDEBUG
    void DebugOut();
#endif

    ///
    bool IsBuildings(int x, int y);

    ///
    void UpdateBuildingsOverview();

    ///
    void UpdatConnectionsOverview();

    ///
    void InitializeOverview();



public:


    int nPopulation;

    int nResidential;
    int nCommercial;
    int nIndustrial;

    int nPowerplant;
    int nPark;
    int nPoliceDept;
    int nFireDept;
    int nStadium;

    int nRoads;
    int nPowerline;

    ///
    CityOverview();

    ///
    void UpdateOverview();
};


#endif // __CITY_OVERVIEW_H



