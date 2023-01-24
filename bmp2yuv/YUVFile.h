#pragma once
#include <vector>


typedef unsigned char BYTE;
typedef unsigned short WORD;

class YUVFile
{
public:
	std::vector<BYTE> raw_data;
	YUVFile(std::vector<BYTE> a_raw_data) : raw_data(a_raw_data)
	{

	}
};