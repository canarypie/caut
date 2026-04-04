#include <vector>
#include <string>
#include <string_view>
#include <algorithm>
#include "utility.h"
#include "fileparser.h"

using namespace FileParser;

FileData FileParser::parse_file(std::string_view path)
{
	std::vector<unsigned char> data = read_buffer(path);
	std::span<unsigned char> buffer = std::span(data);

	if (path.ends_with(".rle")) {
		return FileParser::parse_rle(buffer);
	}

	return FileParser::parse_txt(buffer);
}

FileData FileParser::parse_rle(std::span<unsigned char> file)
{
	//how will we loop through the string? we need the data as a string
	std::string_view buffer(reinterpret_cast<const char*>(file.data()), file.size());

	//skip comment lines
	size_t line_start = 0;
	while (line_start < buffer.size() && buffer[line_start] == '#') {
		line_start = buffer.find('\n', line_start);
		if (line_start == std::string_view::npos) return {};
		line_start++;
	}

	//FileData struct to return
	int width = 0;
	int height = 0;
	std::string rule = "conway";
	std::vector<uint8_t> data;

	//helper functions
	auto is_state_symbol = ([](const unsigned char symbol) -> bool {
		return (symbol == 'o' || symbol == 'b');
	});

	//get header labels for width, height and rule
	auto get_label = ([&](const int pos) -> std::string_view {
		const auto start = buffer.find('=', pos);
		const auto end = buffer.find_first_of(",\n", pos);

		if (start == std::string_view::npos || end <= start) return "0";

		//length of sequence
		const size_t length = (end - start - 1);

		return std::string_view(buffer.substr(start + 1, length));
	});

	const auto header_it = buffer.find('x', line_start);
	if (header_it == std::string_view::npos) return {};
	width = std::stoi(std::string(get_label(header_it)));

	const auto y_pos = buffer.find('y', line_start);
	if (y_pos == std::string_view::npos) return {};
	height = std::stoi(std::string(get_label(y_pos)));

	const auto r_pos = buffer.find("rule", line_start);
	if (r_pos != std::string_view::npos) rule = get_label(r_pos);

	if (width > 0 && height > 0) data.reserve(width * height);

	//get start position - 1 for grid data below header
	const auto data_it = buffer.find('\n', header_it);
	const size_t characters = buffer.size();

	//temporary count for numbers
	int tmp_count = 1;

	for (int i = data_it + 1; i < characters; i++) {
		//only 1 state_symbol = assign 0 or 1 
		if (is_state_symbol(buffer[i])) {
			const auto value = (buffer[i] == 'o') ? 1 : 0;
			data.insert(data.end(), 1, value);
		} else if (buffer[i] == '!') {
			const auto cell_pad = width * height - data.size();
			if (cell_pad > 0) {
				data.insert(data.end(), cell_pad, 0);
			}
		//$ means line break - we need to check for padding to fill the line
		} else if (buffer[i] == '$') {
			const auto row_count = data.size() % width;
			if (row_count > 0) {
				const auto cell_pad = width - row_count;
				data.insert(data.end(), cell_pad, 0);
			}
		//if we record on a digit
		} else if (std::isdigit(buffer[i])) {
			//where the first symbol that isn't a number occurs
			const auto number_end = buffer.find_first_of("ob$", i);
			if (number_end == std::string_view::npos) break;

			const size_t length = number_end - i;

			//tmp count is the converted substring from last to first digit
			tmp_count = std::stoi(std::string(buffer.substr(i, length)));

			//if we have a line skip count, we need to pad and then fill n - 1 lines
			if (buffer[number_end] == '$') {
				const auto row_count = data.size() % width;
				if (row_count > 0) {
					const auto cell_pad = width - row_count;
					data.insert(data.end(), cell_pad, 0);
				}
				data.insert(data.end(), width * (tmp_count - 1), 0);

			//if it's just b or o, we insert the value n times
			} else {
				const auto value = (buffer[number_end] == 'o') ? 1 : 0;
				data.insert(data.end(), tmp_count, value);
			}

			//reset count to 1 (default)
			tmp_count = 1;

			//i is now at the related symbol and gets incremented to the next char
			i = number_end;
		}
	}

	return FileData{width, height, rule, data};
}

//currently only supports direct 0 and 1 files
FileData FileParser::parse_txt(std::span<unsigned char> file)
{
	std::string buffer(reinterpret_cast<const char*>(file.data()), file.size());

	int width = 0;
	int height = 0;

	//should be changeable
	std::string rule = "conway";
	std::vector<uint8_t> data;

	height = std::count(buffer.begin(), buffer.end(), '\n');
	if (file.back() != '\n') height++;

	if (width > 0 && height > 0) data.reserve(width * height);

	std::erase(buffer, '\n');
	std::erase(buffer, '\r');

	width = buffer.size() / height;

	for (const auto& b : buffer){
		data.push_back((b - '0'));	
	}

	return FileData{width, height, rule, data};
}
