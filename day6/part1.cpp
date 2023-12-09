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
	int time;
	int distance;
};

int main(int argc, char **argv)
{
	if (argc != 2) {
		print_usage();
		return 1;
	}

	std::string temp;
	std::string line;
	struct race current_race;
	std::vector<struct race> races;
	std::ifstream fstream(argv[1]);

	// read time line
	std::getline(fstream, line);
	std::stringstream sstream = std::stringstream(line);
	sstream >> temp;	
	while (!sstream.eof()) {
		sstream >> current_race.time;
		races.push_back(current_race);
	}

	int iter = 0;
	std::getline(fstream, line);
	sstream = std::stringstream(line);
	sstream >> temp;
	while (!sstream.eof()) {
		sstream >> races.at(iter++).distance;
	}

	int local_ways;
	int multiplicative_total = 1;
	for (struct race race : races)
	{
		local_ways = 0;
		for (int i = 1; i < race.time - 1; ++i) {
			if (i * (race.time - i) > race.distance) {
				local_ways += 1;
			}
		}
		multiplicative_total *= local_ways;
	}

	std::cout <<  "total: " << multiplicative_total << std::endl;
	return 0;
}
