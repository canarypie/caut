#pragma once

#include <fstream>
#include <cstdint>
#include <iostream>
#include <vector>

std::vector<uint8_t> read_buffer(const char* filePath);
void print_usage();
