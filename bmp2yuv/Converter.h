#pragma once

#include <string>
#include <vector>
#include <stdexcept> 


#include "BMPFile.h"
#include "YUVFile.h"

class Converter
{
	
private:
	// conversion functions (based on http://www.fourcc.org/fccyvrgb.php) <-- domain name is out of date (since AUG 2022)
	static BYTE RGBtoY(BYTE R, BYTE G, BYTE B);
	static BYTE RGBtoU(BYTE R, BYTE G, BYTE B);
	static BYTE RGBtoV(BYTE R, BYTE G, BYTE B);
	static BYTE Clamp(int n);
	static BYTE YUVtoR(BYTE Y, BYTE U, BYTE V);
	static BYTE YUVtoG(BYTE Y, BYTE U, BYTE V);
	static BYTE YUVtoB(BYTE Y, BYTE U, BYTE V);

public:
	static YUVFile RGBtoYUV( BMPFile bmpfile);

};