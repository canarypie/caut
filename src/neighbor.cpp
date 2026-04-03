#include "neighbor.h"
#include <span>

//predefined directions
static constexpr std::array<Offset, 8> MOORE_DIRECTIONS {{
	{-1, -1}, {-1, -0}, {-1, 1},
	{0, -1},           {0, 1},
	{1, -1}, {1, 0}, {1, 1}
}};

static constexpr std::array<Offset, 4> VON_NEUMANN_DIRECTIONS {{
	{-1, 0},
	{0, -1},
	{1, 0},
	{0, 1}
}};

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



Neighbors::Neighbors(const int rows, const int cols, std::string_view neighbors_type)
{
	type = (neighbors_type == "moore") ? NeighborsType::MOORE : NeighborsType::VON_NEUMANN;
	const int range = static_cast<int>(type);

	//intialize indices table to 0
	indices.assign(rows * cols * range, 0);

	preset_rows = rows;
	preset_cols = cols;
	compute_indices();
}

//for when a new pattern is selected
void Neighbors::update(const int rows, const int cols)
{
	//update dimensions
	preset_rows = rows;
	preset_cols = cols;

	compute_indices();
}

//for neighbor type updates
void Neighbors::update(std::string_view neighbors_type)
{	
	type = (neighbors_type == "moore") ? NeighborsType::MOORE : NeighborsType::VON_NEUMANN;
	compute_indices();
}

void Neighbors::compute_indices()
{
	const int range = static_cast<int>(type);

	std::span<const Offset> directions = (type == NeighborsType::MOORE) ? 
		std::span<const Offset>(MOORE_DIRECTIONS) : 
		std::span<const Offset>(VON_NEUMANN_DIRECTIONS);

	for (int y = 0; y < preset_rows; y++) {

		const auto row_index = y * preset_cols;
		 
		for (int x = 0; x < preset_cols; x++) {
			const auto idx = row_index + x;

			int neighbor_indices_idx = 0;
			for (const auto& dir : directions) {
				const auto r = wrap_cell((dir.y + y), preset_rows);
				const auto c = wrap_cell((dir.x + x), preset_cols);

				const auto n_idx = r * preset_cols + c;

				//add to indices table
				indices[idx * range + neighbor_indices_idx] = n_idx;

				neighbor_indices_idx++;
			}
		}
	}
}
