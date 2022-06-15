#include "DrawLCDBitmap.h"


///
DrawLCDBitmap::DrawLCDBitmap(LCDBitmap* bmp)
{
    assert(bmp);

    m_bmp = bmp;
	int dummy;
	gpd->graphics->getBitmapData(m_bmp, &dummy, &dummy, &m_rowBytes, &dummy, &m_buffer);
}


///
DrawLCDBitmap::~DrawLCDBitmap()
{
	if (m_bmp)
	{
		gpd->graphics->freeBitmap(m_bmp);
	}
}


///
void DrawLCDBitmap::PutPixel(uint8_t x, uint8_t y, uint8_t color)
{
    assert(m_buffer != NULL);

	uint8_t* block = m_buffer + (y * m_rowBytes) + (x / 8);
	uint8_t data = 0x80 >> (x % 8);
	*block = color ? *block | data : *block & ~data;
}


///
void DrawLCDBitmap::DrawBitmap(const uint8_t* src, uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
    assert(m_buffer != NULL);

	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			int blockX = i / 8;
			int blockY = j / 8;
			int blocksPerWidth = w / 8;
			int blockIndex = blockY * blocksPerWidth + blockX;
			uint8_t pixels = src[blockIndex * 8 + i % 8];
			uint8_t mask = 1 << (j % 8);
			if (pixels & mask)
			{
				PutPixel(x + i, y + j, 1);
			}
		}
	}
}


///
LCDBitmap* DrawLCDBitmap::GetLCDBitmap()
{
    return m_bmp;
}



///
std::shared_ptr<DrawLCDBitmap> DrawLCDBitmap::CreateInstance(int width, int height, LCDColor bgcolor)
{
    DrawLCDBitmap* p = NULL;
    auto bmp = gpd->graphics->newBitmap(width, height, bgcolor);
    if (bmp)
    {
        p = new DrawLCDBitmap(bmp);
        if (!p)
        {
            gpd->graphics->freeBitmap(bmp);
        }
    }
    return std::shared_ptr<DrawLCDBitmap>(p);
}



