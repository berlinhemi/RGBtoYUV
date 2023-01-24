/*
**  converts *.BMP File to NV12 pixelformat Frame and vice versa
*/

#include <stdio.h>
//#include <io.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
#include <sys/stat.h>  // POSIX

#include <iostream>
#include <fstream>
#include <vector>
//#include <iterator>

#include "BMPFile.h"
#include "YUVFile.h"
#include "IO.h"
#include "Converter.h"
#include "argparse/argparse.h"

//#define HELP "\nrgb2yuv - BMP File (24 bits-per-pixel) to YUV raw Frame \
//\n          (nv12 pixelformat) and vice versa Converter.\n\
//Uses:\n  rgb2yuv image.bmp\nor\n  rgb2yuv WxH frame.yuv\n\
//where:\n  W  - frame width;\n  H  - frame height.\n" 
//
//

////////////////////////////////////////////////////////////////////////////////////

// returns exit code = 0 if success
int main(int argc, char** argv)
{

	argparse::ArgumentParser arg_parcer("bmp2yuv");
	
	arg_parcer.add_argument("--bmp")
		.help("bitmap file name")
		.required();

	try {
		arg_parcer.parse_args(argc, argv);
	}
	catch (const std::runtime_error& err) {
		std::cerr << err.what() << std::endl;
		std::cerr << arg_parcer;
		std::exit(1);
	}

	if (arg_parcer.is_used("--bmp"))
	{
		std::string fname = arg_parcer.get<std::string>("--bmp");  
		try
		{
			BMPFile bmp_data = InputOutput::read_bmpfile(fname);

			YUVFile yuv_data = Converter::RGBtoYUV(bmp_data);

			InputOutput::write_file(fname + ".yuv", yuv_data.raw_data);
		}
		catch (const std::exception& err) {
			std::cerr << err.what() << std::endl;
		}
	}
	else
	{
		std::cerr << arg_parcer.help().str();
	}

	return 0;
} 