#include "system.h"
#include <thread>
#include <chrono>
#include <random>
#include <fstream>
#include <iostream>
#include "utility.h"
#include "arguments.h"

GOLSystem::GOLSystem(const Config& config) : grid(config.rows, config.cols), simulations(config.simulations)
{
	if (!config.file.empty()) {
		auto file = read_buffer(config.file.c_str());
		grid.parse_grid(file);

		grid.data = std::move(file);
	}
	else if (!config.pattern.empty()) load_pattern(config.pattern);

	RenderCfg cfg{};
	cfg.rows = grid.rows;
	cfg.cols = grid.cols;
	cfg.simulations = config.simulations;

	renderer = Renderer(cfg);

	current = std::span<uint8_t>(grid.data);

	//reserve space for future buffer
	future_buf.assign(grid.rows * grid.cols, 0);	

	future = std::span<uint8_t>(future_buf);
}

void GOLSystem::load_pattern(std::string_view pattern)
{
	if (pattern.compare("glider") == 0) init_glider();
	else if (pattern.compare("blinker") == 0) init_blinker();
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
	
	for (int y = 0; y < grid.rows; y++) {
		const auto row_index = y * grid.cols;

		for (int x = 0; x < grid.cols; x++) {
			const auto idx = row_index + x;

			int aliveNeighbors = 0;
			
			for (const auto& dir : directions) {
				const auto r = wrap_cell((dir.y + y), grid.rows);
				const auto c = wrap_cell((dir.x + x), grid.cols);

				const auto n_idx = r * grid.cols + c;

				//only checks the lowest bit for generally living or dying state
				aliveNeighbors += (current[n_idx] & 1);

			}

			future[idx] = (aliveNeighbors == 3) || (aliveNeighbors == 2 && current[idx]);

			
			alive += future[idx];
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
