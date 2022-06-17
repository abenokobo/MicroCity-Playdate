#ifndef __PLAYDATE_MICROCITY_H
#define __PLAYDATE_MICROCITY_H


#include "Common.h"
#include "DrawLCDBitmap.h"
#include "CityInfo.h"
#include "DrawMap.h"



class MicroCity
{
private:


    ///
    static MicroCity m_goInstance;

    ///
    std::shared_ptr<CityInfo> m_spCityInfo;
    
    ///
    std::shared_ptr<DrawMap> m_spDrawMap;

    ///
    std::shared_ptr<DrawLCDBitmap> m_spDraw;

    ///
    MicroCity();



public:


    ///
    void Initialize();

    ///
    void Update();

    ///
    void PutPixel(uint8_t x, uint8_t y, uint8_t color);

    ///
    void DrawBitmap(const uint8_t* data, uint8_t x, uint8_t y, uint8_t w, uint8_t h);

    ///
    void UpdateBuildingScore(Building* building, int score, int crime, int pollution, int localInfluence, int populationEffect, int randomEffect);

    ///
    LCDBitmap* GetMenuBitmap();

    ///
    static MicroCity& GetInstance();
};


#endif // __PLAYDATE_MICROCITY_H



