#pragma once

#include <cstdint>
#include <vector>

struct Grid {
	std::vector<uint8_t> data;

	int rows;
	int cols;

	Grid(const int rows, const int cols);
	void parse_grid(std::vector<uint8_t>& buf);
};
