#pragma once

#include <span>
#include "grid.h"
#include "renderer.h"
#include "arguments.h"


class GOLSystem {
private:
	Grid grid;
	int simulations;
	int gen = 0;
	int alive = 0;

	std::span<uint8_t> current;

	std::vector<uint8_t> future_buf;
	std::span<uint8_t> future;

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
