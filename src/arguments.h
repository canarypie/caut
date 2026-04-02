#pragma once

#include <unistd.h>
#include <string>
#include <string_view>
#include <cstring>

static constexpr int ROWS = 10;
static constexpr int COLS = 10;
static constexpr int SIMULATIONS = 100;
static constexpr std::string_view PATTERN = "random";

// general configuration for our program
struct Config {

	// default dimensions
	int rows = ROWS;
	int cols = COLS;
	
	std::string pattern = std::string(PATTERN);
	std::string file;

	// for later demo mode
	bool demo = false;
	int simulations = SIMULATIONS;

	static Config handle_arguments(const int count, char** arguments);
};

