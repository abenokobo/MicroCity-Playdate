#ifndef __DITHERING_H
#define __DITHERING_H




class Dithering
{
private:


	///
	static inline void SetPixel(unsigned char* dstBuf, int rowBytes, int x, int y)
	{
		unsigned char* nowDst = dstBuf + (rowBytes * y) + (x / 8);
		*nowDst |= 1 << (7 - (x % 8));
	}

	///
	static inline void SetDiffusion(unsigned char* ptr, unsigned char diffNum, int quantError)
	{
		int append = (diffNum * 1.0f) / 16.0f * quantError;
		int newchar = *ptr + append;
		if (newchar >= 0xff)
		{
			*ptr = 0xff;
		}
		else
		if (newchar <= 0x0)
		{
			*ptr = 0x0;
		}
		else
		{
			*ptr = newchar;
		}
	}

	///
	static void FloydSteinbergRaw(unsigned char* srcBuf, int width, int height, unsigned char* dstBuf, int rowBytes, int x, int y);


public:

    /*
		FloydSteinberg
		
        srcBuf:		Source 8bit grayscale bitmap
		width:		Source bitmap width
		height:		Source bitmap height
	    dstBuf:		Destination 1bit binary bitmap (MSB order)
		rowBytes:	Destination bitmap row bytes
	    serpentine:	Use serpentine scanning scanning
    */
	static bool FloydSteinberg(unsigned char* srcBuf, int width, int height, unsigned char* dstBuf, int rowBytes, bool serpentine);
};


#endif  // __DITHERING_H



