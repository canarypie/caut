#pragma once

#include <charconv>
#include <unistd.h>
#include <string>
#include <string_view>
#include <vector>
#include <cstdint>
#include <span>

struct RenderCfg {
	int rows;
	int cols;
	int simulations;

	//escape sequence, header and footer
	static constexpr std::string_view sequence = "\033[2J\033[H";
	static constexpr std::string_view header = "Generation ";
	static constexpr std::string_view footer = "alive: ";
};

class Renderer {
private:
	std::string framebuffer;
	RenderCfg cfg;

public:
	Renderer(const RenderCfg& cfg);
	Renderer() = default;

	void draw(std::span<const uint8_t> grid, const int alive, const int gen);
};
