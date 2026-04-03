#pragma once

#include <span>
#include "grid.h"
#include "renderer.h"
#include "arguments.h"
#include "neighbor.h"


class GOLSystem {
private:
	Grid grid;
	int simulations;
	int gen = 0;
	int alive = 0;

	std::span<uint8_t> current;

	std::vector<uint8_t> future_buf;
	std::span<uint8_t> future;

	Neighbors neighbors;
	Renderer renderer;

public:
	GOLSystem(const Config& config);
	void run();

private:
	void init_glider();
	void init_blinker();
	void init_random();
	void load_pattern(std::string_view pattern);
	void next_generation(std::span<const uint8_t> current);

	void swap_buffers();
};


static constexpr uint8_t states[2][2] = {
	{0, 3}, // for birth state
	{2, 1}  // for survival state
};

static constexpr uint8_t masks[2] = {(1 << 3), (1 << 2) | (1 << 3)}; 
