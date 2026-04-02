#pragma once

#include <fstream>
#include <cstdint>
#include <iostream>
#include <vector>

std::vector<uint8_t> read_buffer(const char* filePath);
int wrap_cell(const int cell, const int limit);
void print_usage();
