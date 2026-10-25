#include "utility.h"
#include <format>
#include <iostream>

std::vector<unsigned char> read_buffer(std::string_view filePath)
{
	std::ifstream file(std::string(filePath), std::ios::ate | std::ios::binary);

	size_t fileSize = (size_t)file.tellg();

	std::vector<unsigned char> buffer(fileSize);

	file.seekg(0);
	file.read((char*)buffer.data(), fileSize);

	return buffer;
}

void write_csv(metrics& metrics, int gen)
{
	// t-s Diagram for analyzing delta t for each simulation round
	// convert time to nanoseconds int
	const auto time_current = std::chrono::steady_clock::now();
	const auto time_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(time_current - metrics.time).count();

	const std::string row = std::format("{},{}\n", gen, time_ms);
	metrics.csv_buf += row;
}

void output_csv(std::string_view csv_buf)
{
	std::ofstream output("output/benchmark.csv");

	output.write(csv_buf.data(), csv_buf.size());
	output.close();
}

void print_usage()
{
	std::cout << "Usage: caut [options] [pattern/file]\n\n"
              << "Options:\n"
              << "  -f <file>       Load pattern from a .txt or .rle file\n"
              << "  -s <num>        Set number of simulations (default: 100)\n"
              << "  -r <num>        Set number of rows (default: 10)\n"
              << "  -c <num>        Set number of cols (default: 10)\n"
			  << "  -p <pattern>    Load a pattern from predefined ones\n"
              << "Patterns: glider, blinker, random (default)\n\n"

			  << "  -R              Change ruleset\n"
			  << "  -N              Change neighbor type\n"
			  << "  -B	        Enable benchmark mode\n"
			  << "  -E              Enable PPM export\n\n"

              << "  -v              Display version information\n"
              << "  -h              Display this help message\n\n";
}
