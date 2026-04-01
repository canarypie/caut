#include <iostream>
#include <span>
#include <cstdint>
#include <array>
#include <chrono>
#include <thread>
#include <string.h>
#include <random>
#include <fstream>

#define ROWS 10
#define COLS 12
#define CELLS (ROWS * COLS)

#define SIMULATIONS 240

/* 1D grid showcase
 *
 * works for every size of rows and columns
 * an array with 3 rows and 2 columns:
 * x x - row 0
 * x x - row 1
 * x x - row 2
 *
 * normally we would iterate through each row and each column inside
 * the objects are stored in a sequential list, we multiply row_index with COLS and add col
 * idx = row_index * COLS + col
 * col = x position, row = y position
 */

// simplified argument name
std::string_view pattern;

// predefined dirs
struct Offset {
	int x;
	int y;
};

static constexpr std::array<Offset, 8> directions = {{
	{-1, -1}, {0, -1}, {1, -1},
	{-1, 0},           {1, 0},
	{-1, 1}, {0, 1}, {1, 1}
}};

class Grid {
	static constexpr int rows = ROWS;
	static constexpr int cols = COLS;
	static constexpr int cells = CELLS;

public:
	std::array<uint8_t, cells> current;
	std::array<uint8_t, cells> future;

};

class GOLSystem {

	Grid grid;


	int gen = 0;

public:
	void init();
	void init_glider();
	void init_blinker();
	void init_random();

	void draw(std::span<const uint8_t> current);
	void next_generation(std::span<const uint8_t> current, std::span<uint8_t> future);
	void swap_buffers();
	void print_grid(std::span<const uint8_t> grid);	
	void run_simulation();

	//buffer pointers for double buffering
	std::span<uint8_t> current;
	std::span<uint8_t> future;

};

int wrap_cell(const int& cell, const int& limit);

// user args
void handle_arguments(const int& count, char** arguments);
std::vector<uint8_t> read_pattern_from_file(const char* filePath);

int main(int argc, char** argv)
{

	handle_arguments(argc, argv);
	
	//read file
	// std::vector<uint8_t> pattern = read_pattern_from_file("glider.txt");


	GOLSystem system;
	system.init();


	system.run_simulation();

	return 0;

}


int wrap_cell(const int& cell, const int& limit)
{
	if (cell < 0) {
		return cell + limit;
	}
	else if (cell >= limit) {
		return cell - limit;
	}
	return cell;
}

// CLASS SYSTEM
void GOLSystem::draw(std::span<const uint8_t> current)
{
	//set terminal properties
	std::cout << "\033[2J\033[H";
	std::cout << "Generation " << gen << '\n';

	print_grid(current);
}

void GOLSystem::print_grid(std::span<const uint8_t> grid)
{
	for (int y = 0; y < ROWS; y++) {
		const auto row_index = y * COLS;
		for (int x = 0; x < COLS; x++) {
			const auto idx = row_index + x;
			std::cout << (grid[idx] ? "■ " : ". ");
		}
		std::cout << '\n';
	}
}

void GOLSystem::swap_buffers()
{
	std::swap(current, future);
}

void GOLSystem::init()
{
	grid.current.fill(0);
	grid.future.fill(0);

	// auto filePattern = read_pattern_from_file("glider.txt");
	// std::copy_n(filePattern.begin(), filePattern.size(), grid.current.begin());

	current = std::span<uint8_t>(grid.current);

	if (pattern.compare("glider") == 0) init_glider();
	// else if (pattern.compare("blinker") == 0) init_blinker();
	// else init_random();

	future = std::span<uint8_t>(grid.future);


	print_grid(current);

}

void GOLSystem::next_generation(std::span<const uint8_t> current, std::span<uint8_t> future)
{
	for (int y = 0; y < ROWS; y++) {
		const auto row_index = y * COLS;

		for (int x = 0; x < COLS; x++) {
			const auto idx = row_index + x;

			int aliveNeighbors = 0;
			
			for (const auto& dir : directions) {
				const auto r = wrap_cell((dir.y + y), ROWS);
				const auto c = wrap_cell((dir.x + x), COLS);

				const auto n_idx = r * COLS + c;
				aliveNeighbors += current[n_idx];

			}
			future[idx] = (aliveNeighbors == 3) || (aliveNeighbors == 2 && current[idx]);
		}
	}

	gen++;

}

void GOLSystem::run_simulation()
{
	for (int i = 0; i < SIMULATIONS; i++) {
		next_generation(current, future);
		swap_buffers();

		draw(current);

		//wait
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

void handle_arguments(const int& count, char** arguments)
{
	if (count == 1) pattern = "random";
	else {
		pattern = arguments[count - 1];
	}
}

void GOLSystem::init_glider()
{
	//Glider pattern
	grid.current[0 * COLS + 1] = 1; 
	grid.current[1 * COLS + 2] = 1; 
	grid.current[2 * COLS + 0] = 1; 
	grid.current[2 * COLS + 1] = 1; 
	grid.current[2 * COLS + 2] = 1;
}

void GOLSystem::init_blinker()
{
    // Blinker pattern (horizontal)
    grid.current[1 * COLS + 0] = 1;  // (1, 0)
    grid.current[1 * COLS + 1] = 1;  // (1, 1)
    grid.current[1 * COLS + 2] = 1;  // (1, 2)
}

void GOLSystem::init_random()
{
	//Random pattern via std::mt19937
	static std::random_device rd;
	static std::mt19937 rng(rd());

	std::bernoulli_distribution dist(0.5f);

	for (int y = 0; y < ROWS; y++) {
		const auto row_index = y * COLS;
		for (int x = 0; x < COLS; x++) {
			const auto idx = row_index + x;

			grid.current[idx] = dist(rng);
		}
	}
}

std::vector<uint8_t> read_pattern_from_file(const char* filePath)
{
	std::ifstream file(filePath, std::ios::ate | std::ios::binary);

	//Get file size
	size_t fileSize = (size_t)file.tellg();

	//buffer
	std::vector<uint8_t> buffer(fileSize / sizeof(uint8_t));	

	//reset cursor for reading file
	file.seekg(0);
	file.read((char*)buffer.data(), fileSize);

	std::erase(buffer, '\n');
	for (auto& b : buffer) {
		b -= '0';
	}

	return buffer;
}
