#include "renderer.h"
#include <format>
#include <fstream>
#include <filesystem>

// get leading zeroes for file name formatting
static int get_padding_width(int num)
{
	if (num < 10) return 1;
	return 1 + get_padding_width(num / 10);	
}

Renderer::Renderer(const RenderCfg& cfg) : cfg(cfg)
{
	framebuffer.reserve((cfg.cols * 4) * cfg.rows + 256);
}

void Renderer::draw(std::span<const uint8_t> grid, const int alive, const int gen)
{
	framebuffer.clear();

	//set terminal properties
	framebuffer += cfg.sequence;
	framebuffer += std::string(cfg.header) + std::to_string(gen) + '\n';

	static constexpr float SCALE = 1.f;

	const int downscale = (int)std::max(1.f, (float)(1.0f/SCALE));
	const int upscale = (int)std::max(1.f, SCALE);

	for (int y = 0; y < cfg.rows; y += downscale) {
		const auto row_index = y * cfg.cols;
		for (int j = 0; j < upscale; j++) {
			for (int x = 0; x < cfg.cols; x += downscale) {
				const auto idx = row_index + x;
				for (int i = 0; i < upscale; i++) {
					framebuffer += color_map[grid[idx]];
				}
			}
			framebuffer += '\n';
		}
	}

	framebuffer += std::string(cfg.footer) + std::to_string(alive) + '\n';

	::write(STDOUT_FILENO, framebuffer.data(), framebuffer.size());

}

void Renderer::output_ppm(std::span<const uint8_t> grid, const int gen)
{
	static constexpr int SCALE = 40;

	//we output each frame as a ppm pixmap image
	//we can use std::ofstream for writing into the output file
	const size_t prefix_padding = get_padding_width(cfg.simulations);
	const std::string prefix = std::format("frame{:0{}}", gen, prefix_padding);
	
	std::string directory = "output/";
	std::string filename = directory + std::string(prefix) + ".ppm";	

	std::filesystem::create_directory(directory);
	std::ofstream output(filename);

	static constexpr auto rgb = ([](const uint8_t state) -> std::string_view {
		static constexpr std::array<std::string_view, 4> rgb_map = {
			"0 0 0", //black (dead cells)
			"0 255 0", //green (alive)
			"255 0 0", //red (dying)
			"0 0 255" //survivor (blue)
		};

		return rgb_map[state];
	});

	output << "P3\n";
	output << cfg.cols * SCALE << ' ' << cfg.rows * SCALE << '\n';
	output << "255" << '\n';
	
	for (int y = 0; y < cfg.rows; y++) {
		const auto row_index = y * cfg.cols;
		for (int j = 0; j < SCALE; j++) {
			for (int x = 0; x < cfg.cols; x++) {
				const auto idx = row_index + x;
				for (int i = 0; i < SCALE; i++) {
					output << rgb(grid[idx]) << " ";
				}
			}
		}

		output << '\n';
	}
	output << '\n';
	
	output.close();
}	
