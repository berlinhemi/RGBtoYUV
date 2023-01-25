#include <fstream>
#include <filesystem>

//#include <exception> 

#include "BMPFile.h"
#include "YUVFile.h"
#include "IO.h"

//bool InputOutput::validate_bmp_headers(const BMPFile& bmp_file)
//{
//
//}

namespace fs = std::filesystem;


BMPFile InputOutput::read_bmpfile(const std::string& file_name)
{
	std::ifstream fi;

	BITMAPFILEHEADER 	kFileHeader;
	BITMAPINFOHEADER 	kInfoHeader;
	int				unWidthAdjusted;
	int				unDataBytes; 
	std::vector<BYTE> pRGBData;

	if (!fs::exists(file_name))
	{
		throw std::runtime_error(std::string("File not found: ") + file_name);
	}

	fi.open(file_name, std::ios::in | std::ios::binary);
	if (!fi.is_open())
	{
		throw std::runtime_error(std::string("Unable to open file: ") + file_name);
	}

	if (!fi.read((char*)&kFileHeader, sizeof(BITMAPFILEHEADER)))
	{
		fi.close();
		throw std::runtime_error(std::string("Unable to read file header from: ") + file_name);
	}

	if (kFileHeader.bfType != (WORD)0x4D42)  // "BM"
	{
		fi.close();
		throw std::runtime_error(std::string("Invalid bitmap file type: ") + file_name);
	}

	if (!fi.read((char*)&kInfoHeader, sizeof(BITMAPINFOHEADER)))
	{
		fi.close();
		throw std::runtime_error(std::string("Unable to read info header:") + file_name);
	}

	if (kInfoHeader.biBitCount != 24)
	{
		fi.close();
		throw std::runtime_error(std::string("Unsupported bitmap pixel format. biBitCount=") +
			std::to_string(kInfoHeader.biBitCount));
	}

	if (kInfoHeader.biCompression != BI_RGB)
	{
		fi.close();
		throw std::runtime_error(std::string("Unsupported bitmap compression. biCompression=") +
			std::to_string(kInfoHeader.biCompression));
	}

	// the Width must be adjusted on int (4 Bytes) boundary
	unWidthAdjusted = kInfoHeader.biWidth * 3;  // line RGB data bytes
	if (unWidthAdjusted % 4)
	{   // if there is reminder from division on 4,
		// then adjust the width with zero bytes
		unWidthAdjusted = (unWidthAdjusted / 4 + 1) * 4;
	}
	unDataBytes = unWidthAdjusted * abs((int)kInfoHeader.biHeight);

	// allocate memory for RGB data
	pRGBData.resize(unDataBytes);
	pRGBData.insert(pRGBData.begin(),
		std::istream_iterator<BYTE>(fi),
		std::istream_iterator<BYTE>());

	fi.close();
	return BMPFile(kFileHeader, kInfoHeader, pRGBData);
	
}
 
void InputOutput::write_file(const std::string& file_name, const std::vector<BYTE>& data)
{
	std::ofstream fo(file_name, std::ios::binary | std::ios::out);
	if (!fo.is_open())
	{
		throw std::runtime_error(std::string("Unable to open file: ") + file_name);
	}
	fo.write(reinterpret_cast<const char*>(&data[0]), data.size());
	if(fo.bad())
	{
		fo.close();
		throw std::runtime_error(std::string("Unable to write file: ") + file_name);
	}
	fo.close();
}