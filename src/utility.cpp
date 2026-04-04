#include "utility.h"

std::vector<unsigned char> read_buffer(std::string_view filePath)
{
	std::ifstream file(std::string(filePath), std::ios::ate | std::ios::binary);

	size_t fileSize = (size_t)file.tellg();

	std::vector<unsigned char> buffer(fileSize);

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
