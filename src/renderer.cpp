#include "renderer.h"

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

	for (int y = 0; y < cfg.rows; y++) {
		const auto row_index = y * cfg.cols;
		for (int x = 0; x < cfg.cols; x++) {
			const auto idx = row_index + x;
			framebuffer += color_map[grid[idx]];
		}
		framebuffer += '\n';
	}

	framebuffer += std::string(cfg.footer) + std::to_string(alive) + '\n';

	::write(STDOUT_FILENO, framebuffer.data(), framebuffer.size());

}
