#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

void print_usage()
{
	std::cout << "Usage: <program> [input-file.txt]" << std::endl;
}

const std::unordered_map<char, int> max_cubes = {
	{ 'r', 12 },
	{ 'g', 13 },
	{ 'b', 14 },
}; 

bool is_valid_game(const std::string &game)
{
	std::istringstream ss(game);
	
	// navigate past header
	ss.ignore(game.length(), ':');

	int count;
	std::string token;
	while (ss >> count >> token)
	{
		if (count > max_cubes.at(token.at(0))) {
			return false;
		}
	}

	return true;
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		print_usage();
		return 1;
	}

	std::string line;
	std::ifstream stream(argv[1]);

	int game_number = 0;
	int running_total = 0;
	while (std::getline(stream, line))
	{
		game_number += 1;
		if (is_valid_game(line)) {
			running_total += game_number;
		}
	}

	std::cout << "running_total = " << running_total << std::endl;
	return running_total;
}
