#include <vector>
#include <string>
#include <string_view>
#include <algorithm>
#include "utility.h"
#include <span>

namespace FileParser {

	// We need a fileparser class for parsing files. The read from file gives us
	// the entire grid, the dimensions and the rule if given (.rle)
	struct FileData {
		int width;
		int height;

		std::string rule = "conway";
		std::vector<uint8_t> grid;
	};

	
	FileData parse_txt(std::span<unsigned char> file);
	FileData parse_rle(std::span<unsigned char> file);

	FileData parse_file(std::string_view);
}
