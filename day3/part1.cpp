#include <fstream>
#include <iostream>
#include <set>
#include <unordered_set>

void print_usage()
{
	std::cout << "Usage: <program> [input-file.txt]" << std::endl;
}

const std::set<char> symbol_set { '#', '$', '%', '&', '*', '+', '-', '/', '=', '@' };
std::unordered_set<long> valid_indices { };

inline bool is_digit(char c)
{
	return c >= 0x30 && c <= 0x39;
}

void add_valid_index(int row, int col)
{
	valid_indices.insert(col | ((long)row << 32));
}

bool is_valid_index(int row, int col)
{
	return valid_indices.find(col | ((long)row << 32)) != valid_indices.cend();
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		print_usage();
		return 1;
	}
	
	int row = 0;
	int running_total = 0;
	std::string line;
	std::unordered_set<std::string> valid_indices;
	std::ifstream stream(argv[1], std::ios::binary);

	// go through and create set of all 'ok' indices
	while (std::getline(stream, line)) 
	{
		for (int col = 0; col < line.length(); ++col)
		{
			if (symbol_set.find(line.at(col)) != symbol_set.cend()) {
				add_valid_index(row - 1, col - 1);
				add_valid_index(row    , col - 1);
				add_valid_index(row + 1, col - 1);
				add_valid_index(row + 1, col    );
				add_valid_index(row + 1, col + 1);
				add_valid_index(row    , col + 1);
				add_valid_index(row - 1, col + 1);
				add_valid_index(row - 1, col    );
			}
		}
		row += 1;
	}

	// clear stream flags and go back to start
	stream.clear();
	stream.seekg(0, std::ios::beg);

	row = 0;
	bool valid_index = false;
	bool reading_digit = false;
	int local_number;

	while (std::getline(stream, line))
	{
		std::cout << "line: " << line << std::endl;
		local_number = 0;
		for (int col = 0; col < line.length(); ++col)
		{
			if (!is_digit(line.at(col)))
			{
				if (reading_digit)
				{
					if (valid_index) {
						std::cout << "  adding value: " << local_number << std::endl;
						running_total += local_number;
					}
					else {
						std::cout << "  NOT adding value: " << local_number << std::endl;
					}
					local_number = 0;
					reading_digit = false;
					valid_index = false;
				}
				else {
					continue;
				}
			}
			else
			{
				reading_digit = true;
				local_number = (local_number * 10) + (line.at(col) - 0x30);
				if (!valid_index && is_valid_index(row, col)) {
					valid_index = true;
				}
			}
		}
		row += 1;
	}

	std::cout << "running_total: " << running_total << std::endl;
	return 0;
}
