#ifndef __MAP_BITMAP_H
#define __MAP_BITMAP_H


#include "Common.h"


class DrawLCDBitmap;



class DrawMap
{
private:

    ///
    static DrawMap m_goInstance;

    ///
    static const int MAP_COUNT = 3;

    ///
    std::shared_ptr<DrawLCDBitmap> m_bmpMaps[MAP_COUNT];

    ///
    LCDBitmap* m_bmpMenu;

    ///
    DrawMap();

    ///
    ~DrawMap();

    ///
    void CreateCityDrawMap(uint8_t terrainType);

    ///
    void DrawMapCursor(int mapLeft, int mapTop);



public:


    ///
    bool Initialize();

    ///
    LCDBitmap* GetLCDBitmap(uint8_t terrainType);

    ///
    LCDBitmap* GetMenuBitmap();

    ///
    static DrawMap& GetInstance();
};


#endif // __MAP_BITMAP_H






