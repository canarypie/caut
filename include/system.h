#pragma once

#include <span>
#include "grid.h"
#include "renderer.h"
#include "arguments.h"
#include "neighbor.h"
#include "ruleset.h"


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
	RuleSet rule;

	Renderer renderer;

public:
	GOLSystem(const Config& config);
	void run();

private:
	void init_glider();
	void init_blinker();
	void init_diamond();
	void init_random();
	void load_pattern(std::string_view pattern);
	void next_generation(std::span<const uint8_t> current);

	void swap_buffers();
};

//struct RuleSet {

//	std::string_view name;

//	//we define a uint16_t lookup table for birth and survival masks
//	//we need uint16_t, since rules like day and night need to shift 8 bits
//	uint16_t masks[2];

//};

static constexpr uint8_t states[2][2] = {
	{0, 3}, // for birth state
	{2, 1}  // for survival state
};

//predefine masks for different rule sets. top is birth rules, bottom is survival rules. Follows B/S notation
// static constexpr uint8_t CONWAY_RULES = {
// 	(1 << 3),
// 	(1 << 2) | (1 << 3)
// };

// static constexpr uint8_t HIGHLIFE_RULES = {
// 	(1 << 3) | (1 << 6),
// 	(1 << 2) | (1 << 3)
// };

// static constexpr uint8_t SEEDS_RULES = {
// 	(1 << 2),
// 	0
// };

// static constexpr uint16_t DAYNIGHT_RULES = {
// 	(1 << 3) | (1 << 6) | (1 << 7) | (1 << 8),
// 	(1 << 3) | (1 << 4) | (1 << 6) | (1 << 7) | (1 << 8)
// };

// static constexpr uint8_t MAZE_RULES = {
// 	(1 << 3),
// 	(1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5)
// };

// static constexpr uint16_t DIAMOEBA_RULES = {
// 	(1 << 3) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8),
// 	(1 << 5) | (1 << 6) | (1 << 7) | (1 << 8)
// };
