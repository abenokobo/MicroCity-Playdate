#ifndef __CITY_INFO_H
#define __CITY_INFO_H


#include "Common.h"
#include "Game.h"


enum CityInfoKind
{
    CityInfoKind_Crime = 0,
    CityInfoKind_Pollution,
    CityInfoKind_COUNT
};


class CityOverview
{
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
    void Initialize();

#ifndef NDEBUG
    void DebugOut();
#endif
};



class CityInfo
{
private:


    ///
    static const int INFO_WIDTH = MAP_WIDTH * PLAYDATE_ZOOM_SCALE;
    static const int INFO_HEIGHT = MAP_HEIGHT * PLAYDATE_ZOOM_SCALE;


    ///
    CityOverview m_oOverview;

    ///
    uint8_t m_buildingScore[MAX_BUILDINGS][CityInfoKind_COUNT];

    ///
    uint8_t m_grayCityInfo[INFO_WIDTH * INFO_HEIGHT];

    ///
    LCDBitmap* m_bmpWork;

    ///
    LCDBitmap* m_bmpCityInfo;
    
    ///
    void ClearCityInfo();

    ///
    void PutCityInfo(uint8_t x, uint8_t y, uint8_t score);

    ///
    void UpdateCityInfo(CityInfoKind kind);



public:

    ///
    CityInfo();

    ///
    void UpdateBuildingScore(Building* building, int score, int crime, int pollution, int localInfluence, int populationEffect, int randomEffect);

    ///
    void DrawCityInfo(CityInfoKind kind);

    ///
    const CityOverview& GetCityOverview();
};



#endif // __CITY_INFO_H







