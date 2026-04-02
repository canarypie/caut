#include "utility.h"

int wrap_cell(const int cell, const int limit)
{
	if (cell < 0) {
		return cell + limit;
	}
	else if (cell >= limit) {
		return cell - limit;
	}
	return cell;
}

std::vector<uint8_t> read_buffer(const char* filePath)
{
	std::ifstream file(filePath, std::ios::ate | std::ios::binary);

	size_t fileSize = (size_t)file.tellg();

	std::vector<uint8_t> buffer(fileSize / sizeof(uint8_t));

	file.seekg(0);
	file.read((char*)buffer.data(), fileSize);

	return buffer;
}

void print_usage()
{
	std::cout << "Usage: ConwayGOL [options] [pattern/file]\n\n"
              << "Options:\n"
              << "  -f <file>       Load pattern from a .txt file\n"
              << "  -s <num>        Set number of simulations (default: 240)\n"
              << "  -r <num>        Set number of rows (default: 20)\n"
              << "  -c <num>        Set number of cols (default: 20)\n"
              << "  -v, --version   Display version information\n"
              << "  -h, --help      Display this help message\n\n"
              << "Patterns: glider, blinker, random (default)\n";
}
