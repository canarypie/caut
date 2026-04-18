#pragma once

#include <fstream>
#include <cstdint>
#include <iostream>
#include <vector>
#include <string_view>
#include <chrono>

struct metrics {
	std::chrono::time_point<std::chrono::steady_clock> time;
	std::string csv_buf;
};

std::vector<unsigned char> read_buffer(std::string_view filePath);
void write_csv(metrics& metrics, int gen);
void output_csv(std::string_view csv_buf);
void print_usage();
