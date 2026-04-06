#pragma once

#include <unistd.h>
#include <string>
#include <string_view>
#include <cstring>

// argument defaults
static constexpr int ROWS = 10;
static constexpr int COLS = 10;
static constexpr int SIMULATIONS = 100;
static constexpr std::string_view PATTERN = "random";
static constexpr std::string_view NEIGHBOR = "moore";
static constexpr std::string_view RULE = "conway";

// general configuration for our program
struct Config {

	// default dimensions
	int rows = ROWS;
	int cols = COLS;
	std::string neighbors_type = std::string(NEIGHBOR);
	std::string rule = std::string(RULE);
	
	std::string pattern = std::string(PATTERN);
	std::string file;

	// for later demo mode
	bool demo = false;
	int simulations = SIMULATIONS;
	bool export_ppm = false;

	static Config handle_arguments(const int count, char** arguments);
};

