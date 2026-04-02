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

	//ansi terminal color string
	static constexpr std::array<std::string_view, 4> color_map = {
        "\033[38;2;40;40;40m. \033[0m",    // 00: dead (grey)
        "\033[38;2;80;255;80m■ \033[0m",   // 10: alive (green)
        "\033[38;2;255;80;80m■ \033[0m",   // 01: dying (red)
        "\033[38;2;0;180;255m■ \033[0m"    // 11: born (blue)
    };
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
