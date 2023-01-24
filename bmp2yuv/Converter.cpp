
#include "Converter.h"


BYTE Converter::RGBtoY(BYTE R, BYTE G, BYTE B)
{
	return (BYTE)((0.257 * R) + (0.504 * G) + (0.098 * B) + 16);
}

 BYTE Converter::RGBtoU(BYTE R, BYTE G, BYTE B)
{
	return (BYTE)(-(0.148 * R) - (0.291 * G) + (0.439 * B) + 128);
}

 BYTE Converter::RGBtoV(BYTE R, BYTE G, BYTE B)
{
	return (BYTE)((0.439 * R) - (0.368 * G) - (0.071 * B) + 128);
}

 BYTE Converter::Clamp(int n)
{
	n &= -(n >= 0);
	return n | ((255 - n) >> 31);
}

 BYTE Converter::YUVtoR(BYTE Y, BYTE U, BYTE V)
{
	int res = (1.164 * (Y - 16) + 1.596 * (V - 128));
	return Clamp(res);
}

 BYTE Converter::YUVtoG(BYTE Y, BYTE U, BYTE V)
{
	int res = (1.164 * (Y - 16) - 0.813 * (V - 128) - 0.391 * (U - 128));
	return Clamp(res);
}

 BYTE Converter::YUVtoB(BYTE Y, BYTE U, BYTE V)
{
	int res = (1.164 * (Y - 16) + 2.018 * (U - 128));
	return Clamp(res);
}

 YUVFile Converter::RGBtoYUV( BMPFile bmpfile)
 {
	 
	 int width = bmpfile.bitmap_infoheader.biWidth;
	 int height = bmpfile.bitmap_infoheader.biHeight;


	 int unDataPixels;
	 int unFrameBytes;
	 int x;
	 int y;
	 // rgb data vars
	 std::vector<BYTE>::iterator pRGBeven;
	 std::vector<BYTE>::iterator pRGBodd;
	 BYTE 	R, G, B;
	 // nv12 data vars
	 std::vector<BYTE>::iterator pYPlane;
	 std::vector<BYTE>::iterator pUVPlane;
	 BYTE 	Y, U, V;
	 BYTE	U00, U01, U10, U11, V00, V01, V10, V11;

	 // check out the rgb data size conditions
	 if ((width % 4) || (height % 4))
	 {
		 throw std::runtime_error(std::string("The Width and Height must be multiple of Four"));
	 }

	 unDataPixels = width * height;
	 // Y plane + UV plane
	 unFrameBytes = (int)(unDataPixels + (unDataPixels / 2));
	 
	 std::vector<BYTE> yuv_data;
	 yuv_data.resize(unFrameBytes);	

	 pYPlane = yuv_data.begin();
	 pUVPlane = yuv_data.begin() + unDataPixels;//unDataPixels - кол-во компонент Y

	 // the algorithm below converts nv12 frame to implement 
	 // the following on-screen line scheme:
	 //
	 //          Y00   Y01  Y02   Y03  ... ...
	 //           U00,V00    U01,V01     ...
	 //          Y10   Y11  Y12   Y13  ... ...
	 //     
	 // in other words, it processes the input rgb data by 
	 // pair of its lines (even & odd at time) to get U & V 
	 // color values from 4 neighbor pixels.

	 for (y = 0; y < height; y += 2)
	 {
		 // set current even and odd line pointers
		 //if (is_bottom_up)
		 //{
		 pRGBeven = bmpfile.raw_pixels.begin() + ((height - y - 1) * (width * 3));
		 pRGBodd = bmpfile.raw_pixels.begin() + ((height - y - 2) * (width * 3));
		 //}
		 /*else
		 {
			 pRGBeven = rgb.begin() + ((y + 0) * (width * 3));
			 pRGBodd = rgb.begin() + ((y + 1) * (width * 3));
		 }*/

		 // process even Y pixels and all U, V pixels
		 for (x = 0; x < width; x += 2)
		 {
			 //*pRGBeven - получение эл-та на который указывает ит-р pRGBeven
			 B = *pRGBeven; 	++pRGBeven;
			 G = *pRGBeven;	++pRGBeven;
			 R = *pRGBeven;  ++pRGBeven;

			 Y = RGBtoY(R, G, B);
			 U00 = RGBtoU(R, G, B);
			 V00 = RGBtoV(R, G, B);

			 *pYPlane = Y;  ++pYPlane;

			 B = *pRGBeven; 	++pRGBeven;
			 G = *pRGBeven;	++pRGBeven;
			 R = *pRGBeven;  ++pRGBeven;

			 Y = RGBtoY(R, G, B);
			 U01 = RGBtoU(R, G, B);
			 V01 = RGBtoV(R, G, B);

			 *pYPlane = Y;  ++pYPlane;

			 B = *pRGBodd; 	++pRGBodd;
			 G = *pRGBodd;	++pRGBodd;
			 R = *pRGBodd;	++pRGBodd;

			 U10 = RGBtoU(R, G, B);
			 V10 = RGBtoV(R, G, B);

			 B = *pRGBodd; 	++pRGBodd;
			 G = *pRGBodd;	++pRGBodd;
			 R = *pRGBodd;	++pRGBodd;

			 U11 = RGBtoU(R, G, B);
			 V11 = RGBtoV(R, G, B);

			 U = (BYTE)((U00 + U01 + U10 + U11) / 4);
			 V = (BYTE)((V00 + V01 + V10 + V11) / 4);

			 *pUVPlane = U;	++pUVPlane;
			 *pUVPlane = V;	++pUVPlane;
		 }

		 // set current odd line pointer back
		 //if (is_bottom_up)
		 //{
		 pRGBodd = bmpfile.raw_pixels.begin() + ((height - y - 2) * (width * 3));
		 //}
		 /*else
		 {
			 pRGBodd = rgb.begin() + ((y + 1) * (width * 3));
		 }*/

		 // process odd Y pixels 
		 for (x = 0; x < width; ++x)
		 {
			 B = *pRGBodd; 	++pRGBodd;
			 G = *pRGBodd;	++pRGBodd;
			 R = *pRGBodd;	++pRGBodd;

			 Y = RGBtoY(R, G, B);

			 *pYPlane = Y;  ++pYPlane;
		 }

	 } // end of for(y)

	 return YUVFile(yuv_data);

  }