#include <iostream>
#include <span>
#include <cstdint>
#include <array>
#include <chrono>
#include <thread>
#include <string.h>
#include <random>
#include <fstream>
#include <unistd.h>


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

static constexpr int ROWS = 10;
static constexpr int COLS = 10;
static constexpr int SIMULATIONS = 100;
static constexpr std::string PATTERN = "random";

// static constexpr std::string ANSI_ESCAPE = "\033[2J\033[H";

// general configuration for our program
struct Config {

	// default dimensions
	int rows = ROWS;
	int cols = COLS;
	
	std::string pattern = PATTERN;
	std::string file;

	// for later demo mode
	bool demo = false;
	int simulations = SIMULATIONS;

	static Config handle_arguments(const int count, char** arguments);
};


// predefined dirs we can just loop through without nested looping
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
public:
	std::vector<uint8_t> current;
	std::vector<uint8_t> future;

	Grid(const int rows, const int cols);

	int rows;
	int cols;
	int alive = 0;
};

class GOLSystem {
private:
	Grid grid;
	int simulations;
	int gen = 0;

	std::string write_buffer;

public:
	GOLSystem(const Config& config);
	
	std::span<uint8_t> current;
	std::span<uint8_t> future;

	void draw(std::span<const uint8_t> current);
	void next_generation(std::span<const uint8_t> current);
	void swap_buffers();
	void reset();
	void run();

private:
	void init_glider();
	void init_blinker();
	void init_random();
	void load_pattern(std::string_view pattern);

	void print_grid(std::span<const uint8_t> current);	
	void parse_grid(std::vector<uint8_t>& buf);

};

int wrap_cell(const int& cell, const int& limit);
std::vector<uint8_t> read_buffer(const char* filePath);
static void print_usage();

// user args

int main(int argc, char** argv)
{

	auto config = Config::handle_arguments(argc, argv);
	//read file
	// std::vector<uint8_t> pattern = read_pattern_from_file("glider.txt");

	GOLSystem system(config);
	// system.init();

	system.run();

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
	write_buffer.clear();

	//set terminal properties
	write_buffer += "\033[2J\033[H";
	write_buffer += "Generation " + std::to_string(gen) + '\n';

	print_grid(current);

	write_buffer += "alive: " + std::to_string(grid.alive) + '\n';

	::write(STDOUT_FILENO, write_buffer.data(), write_buffer.size());
}

void GOLSystem::print_grid(std::span<const uint8_t> current)
{
	for (int y = 0; y < grid.rows; y++) {
		const auto row_index = y * grid.cols;
		for (int x = 0; x < grid.cols; x++) {
			const auto idx = row_index + x;
			write_buffer += (current[idx] ? "■ " : ". ");
		}
		write_buffer += '\n';
	}
}

void GOLSystem::swap_buffers()
{
	std::swap(current, future);
}

void GOLSystem::next_generation(std::span<const uint8_t> current)
{
	grid.alive = 0;

	for (int y = 0; y < grid.rows; y++) {
		const auto row_index = y * grid.cols;

		for (int x = 0; x < grid.cols; x++) {
			const auto idx = row_index + x;

			int aliveNeighbors = 0;
			
			for (const auto& dir : directions) {
				const auto r = wrap_cell((dir.y + y), grid.rows);
				const auto c = wrap_cell((dir.x + x), grid.cols);

				const auto n_idx = r * grid.cols + c;
				aliveNeighbors += current[n_idx];

			}
			future[idx] = (aliveNeighbors == 3) || (aliveNeighbors == 2 && current[idx]);
			grid.alive += future[idx];
		}
	}

	gen++;

}

void GOLSystem::run()
{
	for (int i = 0; i < simulations; i++) {
		next_generation(current);

		if (grid.alive == 0) {
			std::cout << "No cells alive!\n";
			exit(0);
		}

		swap_buffers();
		draw(current);

		//wait
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

void GOLSystem::load_pattern(std::string_view pattern)
{
	// for later checking through a map
	if (pattern.compare("glider") == 0) init_glider();
	else if (pattern.compare("blinker") == 0) init_blinker();
	else init_random();
}

void GOLSystem::init_glider()
{
	//Glider pattern
	grid.current[0 * grid.cols + 1] = 1; 
	grid.current[1 * grid.cols + 2] = 1; 
	grid.current[2 * grid.cols + 0] = 1; 
	grid.current[2 * grid.cols + 1] = 1; 
	grid.current[2 * grid.cols + 2] = 1;
}

void GOLSystem::init_blinker()
{
    // Blinker pattern (horizontal)
    grid.current[1 * grid.cols + 0] = 1;  // (1, 0)
    grid.current[1 * grid.cols + 1] = 1;  // (1, 1)
    grid.current[1 * grid.cols + 2] = 1;  // (1, 2)
}

void GOLSystem::init_random()
{
	//Random pattern via std::mt19937
	static std::random_device rd;
	static std::mt19937 rng(rd());

	std::bernoulli_distribution dist(0.5f);

	for (int y = 0; y < grid.rows; y++) {
		const auto row_index = y * grid.cols;
		for (int x = 0; x < grid.cols; x++) {
			const auto idx = row_index + x;

			grid.current[idx] = dist(rng);
		}
	}
}

std::vector<uint8_t> read_buffer(const char* filePath)
{
	std::ifstream file(filePath, std::ios::ate | std::ios::binary);

	//Get file size
	size_t fileSize = (size_t)file.tellg();

	//buffer
	std::vector<uint8_t> buffer(fileSize / sizeof(uint8_t));	

	//reset cursor for reading file
	file.seekg(0);
	file.read((char*)buffer.data(), fileSize);

	return buffer;
}

GOLSystem::GOLSystem(const Config& config) : grid(config.rows, config.cols), simulations(config.simulations)
{

	if (!config.file.empty()) {
		auto file = read_buffer(config.file.c_str());
		parse_grid(file);

		grid.current = std::move(file);
		grid.future.assign(grid.rows * grid.cols, 0);
			
	}
	else if (!config.pattern.empty()) load_pattern(config.pattern);


	current = std::span<uint8_t>(grid.current);
	future = std::span<uint8_t>(grid.future);
	
}

void GOLSystem::parse_grid(std::vector<uint8_t>& buf)
{
	//here we parse the actual grid pattern from our buffer
	//find row count
	
	grid.rows = std::count(buf.begin(), buf.end(), '\n');
	if (buf.back() != '\n') grid.rows++;

	//get actual element size after removing trailing characters first
	std::erase(buf, '\n');
	std::erase(buf, '\r');

	grid.cols = buf.size() / grid.rows;

	//initialize grid to integer 0
	for (auto& b : buf) {
		b -= '0';
	}
}


Grid::Grid(const int rows, const int cols) : rows(rows), cols(cols)
{
	current.assign(rows * cols, 0);
	future.assign(rows * cols, 0);
}

Config Config::handle_arguments(const int count, char** arguments)
{
	Config config;

	int opt;
	while ((opt = getopt(count, arguments, "p:f:s:r:c:vh")) != -1)
	{
		switch (opt)
		{
			case 'p':
			{
				config.pattern = optarg;
				break;
			}
			case 'f':
			{
				config.file = optarg;
				break;
			}
			case 'v':
				std::cout << "ConwayGOL version " << GOL_VERSION << '\n'; 
				exit(1);
			case 'h':
				print_usage();
				exit(1);
			case 's':
			{
				config.simulations = std::stoi(optarg);
				break;
			}
			case 'r': 
			{
				config.rows = std::stoi(optarg);
				break;
			}
			case 'c':
			{
				config.cols = std::stoi(optarg);
				break;
			}
			default:
			{
				std::cout << "Unknown argument" << std::endl;
			}
		}
	}
	return config;
}

static void print_usage()
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
