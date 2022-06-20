#ifndef __PLAYDATE_MICROCITY_H
#define __PLAYDATE_MICROCITY_H


#include "Common.h"
#include "DrawLCDBitmap.h"
#include "Menu.h"
#include "CityInfo.h"
#include "BuildingScore.h"



class MicroCity
{
private:


    ///
    static std::shared_ptr<MicroCity> m_gspInstance;

    ///
    bool m_bStarted;

    ///
    std::shared_ptr<CityInfo> m_spCityInfo;

    ///
    std::shared_ptr<BuildingScore> m_spBuildingScore;

    ///
    std::shared_ptr<Menu> m_spMenu;

    ///
    std::shared_ptr<DrawLCDBitmap> m_spDrawDisplay;

    ///
    MicroCity();



public:


    ///
    ~MicroCity();

    ///
    void Initialize();

    ///
    void Update();

    ///
    void OnPause();

    ///
    void OnMenuCityInfo();

    ///
    void OnExitCityInfo();

    ///
    void PutPixel(uint8_t x, uint8_t y, uint8_t color);

    ///
    void DrawBitmap(const uint8_t* data, uint8_t x, uint8_t y, uint8_t w, uint8_t h);

    ///
    void UpdateBuildingScore(Building* building, int score, int crime, int pollution, int localInfluence, int populationEffect, int randomEffect);

    ///
    static MicroCity& GetInstance();

    ///
    static void Finalize();
};


#endif // __PLAYDATE_MICROCITY_H



