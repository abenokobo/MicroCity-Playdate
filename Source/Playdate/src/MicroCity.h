#ifndef __PLAYDATE_MICROCITY_H
#define __PLAYDATE_MICROCITY_H


#include "Common.h"
#include "DrawLCDBitmap.h"


class MicroCity
{
private:


    ///
    static MicroCity m_goInstance;

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
    LCDBitmap* GetMenuBitmap();

    ///
    static MicroCity& GetInstance();
};


#endif // __PLAYDATE_MICROCITY_H



