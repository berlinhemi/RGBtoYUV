#pragma once

#include <string>
#include <vector>

#include "BMPFile.h"
#include "YUVFile.h"


class InputOutput
{

public:
	static BMPFile read_bmpfile(const std::string& file_name);
	static void write_file(const std::string& file_name, const std::vector<BYTE>& data);
};
