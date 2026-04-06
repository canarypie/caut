#include "ruleset.h"
#include <thread>
#include <chrono>
#include <iostream>

RuleSet::RuleSet(std::string_view rule) : rule(rule)
{
	const auto it = rule_presets.find(rule);

	if (it != rule_presets.end()) {
		//here we should assign it manually per index becase of register caching
		masks[0] = it->second[0];
		masks[1] = it->second[1];
	}
	else {
		masks = parse_rule(rule);
	}
}


void RuleSet::update(std::string_view rule)
{
	this->rule = rule;
	masks = parse_rule(rule);

}

//for B/S notation
std::array<uint16_t, 2> RuleSet::parse_rule(std::string_view rule)
{
	uint16_t birth_mask;
	uint16_t survival_mask;

	//split into 2 substrings
	std::string_view birth_rule = rule.substr(0, rule.find('/'));
	std::string_view survival_rule = rule.substr(rule.find('/') + 1);
	
	static constexpr auto set_mask = ([](std::string_view rule_view) {
		uint16_t mask = 0;

		for (int i = 0; i < rule_view.length(); i++) {
			if (!std::isdigit(rule_view[i])) continue;

			mask |= (1 << (rule_view[i] - '0'));
		}

		return mask;
	});

	birth_mask = set_mask(birth_rule);
	survival_mask = set_mask(survival_rule);

	return {birth_mask, survival_mask};
}
