#ifndef __DRAW_LCDBITMAP_H
#define __DRAW_LCDBITMAP_H


#include "Common.h"



class DrawLCDBitmap
{
private:


    ///
    LCDBitmap* m_bmp;

    ///
    uint8_t* m_buffer;

    ///
    int m_rowBytes;

    ///
    DrawLCDBitmap(LCDBitmap* bmp);



public:

    ///
    ~DrawLCDBitmap();

    ///
    void PutPixel(uint8_t x, uint8_t y, uint8_t color);

    ///
    void DrawBitmap(const uint8_t* src, uint8_t x, uint8_t y, uint8_t w, uint8_t h);

    ///
    LCDBitmap* GetLCDBitmap();

    ///
    static std::shared_ptr<DrawLCDBitmap> CreateInstance(int width, int height, LCDColor bgcolor);
};


#endif // __DRAW_LCDBITMAP_H






