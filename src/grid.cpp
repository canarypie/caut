#include "grid.h"
#include <algorithm>

Grid::Grid(const int rows, const int cols) : rows(rows), cols(cols)
{
	data.assign(rows * cols, 0);
}

void Grid::parse_grid(std::vector<uint8_t>& buf)
{
	//here we parse our actual grid pattern on the buffer
	//find row count
	rows = std::count(buf.begin(), buf.end(), '\n');
	if (buf.back() != '\n') rows++;

	std::erase(buf, '\n'),
	std::erase(buf, '\r'),

	cols = buf.size() / rows;

	//initialize grid to integer values from chars
	for (auto& b : buf) {
		b -= '0';
	}	
}
