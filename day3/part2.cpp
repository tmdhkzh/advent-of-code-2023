#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

void print_usage()
{
	std::cout << "Usage: <program> [input-file.txt]" << std::endl;
}

std::unordered_map<long, long> index_to_gear { };
std::unordered_map<long, std::vector<int>> gear_ratios { };

inline bool is_digit(char c)
{
	return c >= 0x30 && c <= 0x39;
}

inline long xy_to_long(int row, int col)
{
	return col | ((long)row << 32);
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		print_usage();
		return 1;
	}

	int row = 0;
	long gear_key;
	std::string line;
	std::ifstream stream(argv[1], std::ios::binary);
	while (std::getline(stream, line))
	{
		for (int col = 0; col < line.length(); ++col)
		{
			if (line.at(col) != '*') {
				continue;
			}

			// overwriting is okay
			gear_key = xy_to_long(row, col);
			gear_ratios[gear_key] = std::vector<int> { };
			index_to_gear[xy_to_long(row - 1, col - 1)] = gear_key;
			index_to_gear[xy_to_long(row    , col - 1)] = gear_key;
			index_to_gear[xy_to_long(row + 1, col - 1)] = gear_key;
			index_to_gear[xy_to_long(row + 1, col    )] = gear_key;
			index_to_gear[xy_to_long(row + 1, col + 1)] = gear_key;
			index_to_gear[xy_to_long(row    , col + 1)] = gear_key;
			index_to_gear[xy_to_long(row - 1, col + 1)] = gear_key;
			index_to_gear[xy_to_long(row - 1, col    )] = gear_key;
		}
		row += 1;
	}

	int local_number;
	bool is_gear_member = false;
	bool reading_digit = false;

	row = 0;
	stream.clear();
	stream.seekg(0, std::ios::beg);	
	while (std::getline(stream, line))
	{	
		gear_key = -1;
		local_number = 0;
		for (int col = 0; col < line.length(); ++col)
		{
			if (!is_digit(line.at(col)))
			{
				if (reading_digit)
				{
					if (gear_key != -1) {
						gear_ratios.at(gear_key).push_back(local_number);
					}
					local_number = 0;
					gear_key = -1;
					reading_digit = false;
				}
				else {
					continue;
				}
			}
			else
			{
				reading_digit = true;
				local_number = (local_number * 10) + (line.at(col) - 0x30);
				if (gear_key == -1)
				{
					auto iter = index_to_gear.find(xy_to_long(row, col));
					if (iter != index_to_gear.end()) {
						gear_key = iter->second;
					}
				}	
			}
		}
		// check for number that terminates at the end of the line
		if (reading_digit && gear_key != -1) {
			gear_ratios.at(gear_key).push_back(local_number);
			reading_digit = false;
		}
		row += 1;
	}
	
	int local_gear_ratio;
	long gear_ratio_total = 0;
	for (const std::pair<long, std::vector<int>> &gear_ratio : gear_ratios)
	{
		if (gear_ratio.second.size() != 2) {
			continue;
		}
		gear_ratio_total += gear_ratio.second.at(0) * gear_ratio.second.at(1);
	}

	std::cout << "gear ratio total: " << gear_ratio_total << std::endl;
	return 0;
}
