#ifndef __MAP_SCORE_H
#define __MAP_SCORE_H


#include "Common.h"
#include "Game.h"


enum BuildingScoreKind
{
    BuildingScoreKind_PopulationDestiny = 0,
    BuildingScoreKind_Crime,
    BuildingScoreKind_Pollution,
    BuildingScoreKind_COUNT
};


class BuildingScore
{
private:


    ///
    static const int BS_WIDTH = MAP_WIDTH * PLAYDATE_ZOOM_SCALE;
    static const int BS_HEIGHT = MAP_HEIGHT * PLAYDATE_ZOOM_SCALE;


    ///
    uint8_t m_buildingScore[MAX_BUILDINGS][BuildingScoreKind_COUNT];

    ///
    uint8_t m_grayBuildingScore[BS_WIDTH * BS_HEIGHT];

    ///
    LCDBitmap* m_bmpWork;

    ///
    LCDBitmap* m_bmpBuildingScore;
    
    ///
    void ClearBuildingScore();

    ///
    void PutBuildingScore(uint8_t x, uint8_t y, uint8_t score);

    ///
    void UpdateBuildingScore(BuildingScoreKind kind);



public:

    ///
    BuildingScore();

    ///
    ~BuildingScore();

    ///
    void UpdateBuildingScore(Building* building, int score, int crime, int pollution, int localInfluence, int populationEffect, int randomEffect);

    ///
    void DrawBuildingScore(BuildingScoreKind kind, uint8_t x, uint8_t y);
};



#endif // __MAP_SCORE_H







