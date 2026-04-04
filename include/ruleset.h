#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <cstdint>
#include <array>


//predefine masks for different rule sets. top is birth rules, bottom is survival rules
static constexpr uint16_t CONWAY_RULES[] = {
	(1 << 3),
	(1 << 2) | (1 << 3)
};

static constexpr uint16_t HIGHLIFE_RULES[] = {
	(1 << 3) | (1 << 6),
	(1 << 2) | (1 << 3)
};

static constexpr uint16_t SEEDS_RULES[] = {
	(1 << 2),
	0
};

static constexpr uint16_t DAYNIGHT_RULES[] = {
	(1 << 3) | (1 << 6) | (1 << 7) | (1 << 8),
	(1 << 3) | (1 << 4) | (1 << 6) | (1 << 7) | (1 << 8)
};

static constexpr uint16_t MAZE_RULES[] = {
	(1 << 3),
	(1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5)
};

static constexpr uint16_t DIAMOEBA_RULES[] = {
	(1 << 3) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8),
	(1 << 5) | (1 << 6) | (1 << 7) | (1 << 8)
};


class RuleSet {
	
public:
	std::string rule;
	std::array<uint16_t, 2> masks{0};

	RuleSet() = default;
	RuleSet(std::string_view rule);

	//for current rule names
	void update(std::string_view rule);


private:
	inline static std::unordered_map<std::string_view, const uint16_t*> rule_presets = 
	{
		{"conway", CONWAY_RULES},
		{"highlife", HIGHLIFE_RULES},
		{"seeds", SEEDS_RULES},
		{"daynight", DAYNIGHT_RULES},
		{"maze", MAZE_RULES},
		{"diamoeba", DIAMOEBA_RULES}
	};

	std::array<uint16_t, 2> parse_rule(std::string_view rule);
};

