#include "system.h"
#include <thread>
#include <chrono>
#include <random>
#include <fstream>
#include <iostream>
#include "utility.h"
#include "arguments.h"
#include "ruleset.h"

GOLSystem::GOLSystem(const Config& config) : grid(config.rows, config.cols), simulations(config.simulations)
{
	if (!config.file.empty()) {
		auto file = read_buffer(config.file.c_str());
		grid.parse_grid(file);

		grid.data = std::move(file);
	}
	else if (!config.pattern.empty()) load_pattern(config.pattern);


	current = std::span<uint8_t>(grid.data);
	future_buf.assign(grid.rows * grid.cols, 0);	
	future = std::span<uint8_t>(future_buf);

	//intialize neighbor indices table
	neighbors = Neighbors(grid.rows, grid.cols, config.neighbors_type);

	rule = RuleSet(config.rule);

	//prepare renderer
	RenderCfg cfg{};
	cfg.rows = grid.rows;
	cfg.cols = grid.cols;
	cfg.simulations = config.simulations;

	renderer = Renderer(cfg);
}

void GOLSystem::load_pattern(std::string_view pattern)
{
	if (pattern.compare("glider") == 0) init_glider();
	else if (pattern.compare("blinker") == 0) init_blinker();
	else if (pattern == "diamond") init_diamond();
	else init_random();
}

void GOLSystem::init_glider()
{
	//Glider pattern
	grid.data[0 * grid.cols + 1] = 1; 
	grid.data[1 * grid.cols + 2] = 1; 
	grid.data[2 * grid.cols + 0] = 1; 
	grid.data[2 * grid.cols + 1] = 1; 
	grid.data[2 * grid.cols + 2] = 1;
}

void GOLSystem::init_blinker()
{
	// Blinker pattern (horizontal)
	grid.data[1 * grid.cols + 0] = 1;  // (1, 0)
    grid.data[1 * grid.cols + 1] = 1;  // (1, 1)
    grid.data[1 * grid.cols + 2] = 1;  // (1, 2)
}

void GOLSystem::init_diamond()
{
	//Diamond pattern that expands recursively with Von Neumann Neighbors
	const auto row_half = grid.rows / 2;
	const auto col_half = grid.cols / 2;

	grid.data[row_half * grid.cols + col_half] = 1;
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

			grid.data[idx] = dist(rng);
		}
	}
}

void GOLSystem::next_generation(std::span<const uint8_t> current)
{
	alive = 0;
	
	const int range = static_cast<int>(neighbors.type);

	//outer loop for tiling the grid
	for (int ty = 0; ty < grid.rows; ty += 32) {
		for (int tx = 0; tx < grid.cols; tx += 32) {

			//inner loop for looping through the grid in chunks
			//we control how many rows/cols we iterate through at once
			//we do this, because our cache access is bottlenecked if the data gets larger than L1/L2 sizes

			int row_limit = std::min(grid.rows, ty + 32);
			int col_limit = std::min(grid.cols, tx + 32);

			for (int y = ty; y < row_limit; y++) {
				const auto row_index = y * grid.cols;

				for (int x = tx; x < col_limit; x++) {
					const auto idx = row_index + x;

					int aliveNeighbors = 0;

					//precomputed indices lookup table
					for (int i = 0; i < range; i++)
					{
						aliveNeighbors += (current[neighbors.indices[idx * range + i]] & 1);
					}

					int is_alive = (current[idx] & 1);

					int will_live ((rule.masks[is_alive] >> aliveNeighbors) & 1);

					future[idx] = states[is_alive][will_live];

					alive += (future[idx] & 1);
				}
			}
		}
	}

	gen++;
}

void GOLSystem::run()
{
	for (int i = 0; i < simulations; i++) {
		next_generation(current);

		if (alive == 0) {
			std::cout << "No cells alive!\n";
			exit(1);
		}

		swap_buffers();
		renderer.draw(current, alive, gen);

		//wait
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

void GOLSystem::swap_buffers()
{
	std::swap(current, future);
}
