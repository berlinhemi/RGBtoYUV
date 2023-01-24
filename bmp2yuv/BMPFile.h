#pragma once
#include <vector>


typedef unsigned char BYTE;
typedef unsigned short WORD;
#define BI_RGB 		0x0000

#pragma pack(push, 2)
typedef struct
{
	unsigned short    bfType;					// The file type; must be BM.
	int				  bfSize;					// The size, in bytes, of the bitmap file.
	unsigned short    bfReserved1;
	unsigned short    bfReserved2;
	int				  bfOffBits;				// The offset, in bytes, to the bitmap bits.
									//  for us = sizeof(BITMAPFILEHEADER) + 
									//  sizeof(BITMAPINFOHEADER)
} BITMAPFILEHEADER;

typedef struct
{
	int      biSize;				// sizeof(BITMAPINFOHEADER)
	int      biWidth;				// The width of the bitmap, in pixels.
	int      biHeight;			// The height of the bitmap, in pixels.
									//  If biHeight is positive, the bitmap is a 
									//  bottom-up DIB. If biHeight is negative, 
									//  the bitmap is a top-down DIB.
	unsigned short       biPlanes;			// This value must be set to 1.
	unsigned short       biBitCount;          // Bits-per-pixel (for us = 24).
	int      biCompression;		// The type of compression (for us = BI_RGB,
									//  an uncompressed format.)
	int      biSizeImage;			// The size, in bytes, of the image. This may 
									//  be set to zero for BI_RGB bitmaps.
	int      biXPelsPerMeter;		// must be 0;
	int      biYPelsPerMeter;     // must be 0;
	int      biClrUsed;			// must be 0;
	int      biClrImportant;		// must be 0;
} BITMAPINFOHEADER;
#pragma pack(pop)

class BMPFile
{
public:
	BITMAPFILEHEADER bitmap_fileheader;
	BITMAPINFOHEADER bitmap_infoheader;
	std::vector<BYTE> raw_pixels;
	BMPFile(BITMAPFILEHEADER fh, 
		BITMAPINFOHEADER ih, 
		std::vector<BYTE> a_raw_data) : bitmap_fileheader(fh), 
										bitmap_infoheader(ih),  
										raw_pixels(a_raw_data)
	{

	}
};