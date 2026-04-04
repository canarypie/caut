#pragma once

#include <fstream>
#include <cstdint>
#include <iostream>
#include <vector>
#include <string_view>

std::vector<unsigned char> read_buffer(std::string_view filePath);
void print_usage();
