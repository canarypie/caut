#pragma once

#include <string_view>
#include <cstdint>
#include <vector>
#include <array>


//reserve maximum space for demo mode without needs for dynamic resizing
static constexpr int MAX_GRID_SIZE = 1000 * 1000 * 8;

enum class NeighborsType {
  MOORE = 8,
  VON_NEUMANN = 4
};


class Neighbors
{
public:
	// indices LUT
	std::vector<int32_t> indices;
	NeighborsType type;

	int preset_rows;
	int preset_cols;

	Neighbors() = default;
	Neighbors(const int rows, const int cols, std::string_view neighbors_type);
	
	//update function only available in demo mode
	//only switch between neighbor types
	void update(std::string_view neighbors_type);
	void update(const int rows, const int cols);

private:
	void compute_indices();
};

struct Offset {
	int x;
	int y;
};
