#include "Dithering.h"


///
void Dithering::FloydSteinbergRaw(unsigned char* srcBuf, int width, int height, unsigned char* dstBuf, int rowBytes, int x, int y)
{
    signed int quantError;
    unsigned char oldPixel = srcBuf[x + (y * width)];
    if (oldPixel > 0x7f)
    {
        quantError = oldPixel - 0xff;
        SetPixel(dstBuf, rowBytes, x, y);
    }
    else
    {
        quantError = oldPixel;
    }

    /*
        | |X|7|
        |3|5|1|
    */
    if (x != width - 1)
    {
        SetDiffusion(srcBuf + (x + 1) + (y * width), 7, quantError);
    }
    
    if ((x != 0) && (y != height - 1))
    {
        SetDiffusion(srcBuf + (x - 1) + ((y + 1) * width), 3, quantError);
    }
    
    if (y != height - 1)
    {
        SetDiffusion(srcBuf + x + ((y + 1) * width), 5, quantError);
    }
    
    if ((x != width - 1) && (y != height - 1))
    {
        SetDiffusion(srcBuf + (x + 1) + ((y + 1) * width), 1, quantError);
    }
}


///
bool Dithering::FloydSteinberg(unsigned char* srcBuf, int width, int height, unsigned char* dstBuf, int rowBytes, bool serpentine)
{
    int y = 0;
    for (y = 0; y < height; y++)
    {
        if (!serpentine || (y % 2 == 0))
        {
            for (int x = 0; x < width; x++)
            {
                FloydSteinbergRaw(srcBuf, width, height, dstBuf, rowBytes, x, y);
            }
        }
        else
        {
            for (int x = width  -1; x >= 0; x--)
            {
                FloydSteinbergRaw(srcBuf, width, height, dstBuf, rowBytes, x, y);
            }
        }
    }

    return true;
}
