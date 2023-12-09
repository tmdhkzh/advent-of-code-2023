#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void print_usage()
{
	std::cout << "Usage: <program> [input-file.txt]" << std::endl;
}

struct race {
	uint64_t time;
	uint64_t distance;
};

inline bool is_digit(char c)
{
	return c >= 0x30 && c <= 0x39;
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		print_usage();
		return 1;
	}

	char c;
	std::string temp;
	std::string line;
	struct race current_race { 0, 0 };
	std::ifstream fstream(argv[1]);

	// read time line
	std::getline(fstream, line);
	std::stringstream sstream = std::stringstream(line);
	sstream >> temp;	
	while (!sstream.eof()) {
		sstream >> c;
		if (is_digit(c)) {
			current_race.time = (current_race.time * 10) + (c - 0x30);
		}
	}

	std::getline(fstream, line);
	sstream = std::stringstream(line);
	sstream >> temp;
	while (!sstream.eof()) {
		sstream >> c;
		if (is_digit(c)) {
			current_race.distance = (current_race.distance * 10) + (c - 0x30);
		}
	}

	current_race.time /= 10;
	current_race.distance /= 10;
	std::cout << "race: time=" << current_race.time << ", dist=" << current_race.distance << std::endl;

	int upper = 0;
	int lower = 0;
	for (uint64_t i = 0; i < current_race.time; ++i)
	{
		if ((current_race.time - i) * i > current_race.distance) {
			lower = i;
			break;
		}
	}
	for (uint64_t i = current_race.time - 1; i >= 0; --i) {
		if ((current_race.time - i) * i > current_race.distance) {
			upper = i;
			break;
		}
	}

	std::cout << "upper=" << upper << ", lower=" << lower << ", count=" << upper - lower + 1 << std::endl;
	return 0;
}
