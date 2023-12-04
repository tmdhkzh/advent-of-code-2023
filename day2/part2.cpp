#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

std::unordered_map<char, int> minimum_counts;

void print_usage()
{
	std::cout << "Usage: <program> [input-file.txt]" << std::endl;
}

inline void clear_counts()
{
	minimum_counts['r'] = 0;
	minimum_counts['g'] = 0;
	minimum_counts['b'] = 0;
}

inline int get_power_value()
{
	return minimum_counts['r']
			* minimum_counts['g']
			* minimum_counts['b'];
}

int get_minimum_counts(const std::string &game)
{
	std::istringstream ss(game);
	clear_counts();
	
	// navigate past header
	ss.ignore(game.length(), ':');

	int count;
	std::string token;
	std::unordered_map<char, int>::iterator pos;
	while (ss >> count >> token)
	{
		pos = minimum_counts.find(token.at(0));
		pos->second = std::max(count, pos->second);
	}

	return get_power_value();
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		print_usage();
		return 1;
	}

	std::string line;
	std::ifstream stream(argv[1]);

	int running_total = 0;
	while (std::getline(stream, line))
	{
		running_total += get_minimum_counts(line);
	}

	std::cout << "running_total = " << running_total << std::endl;
	return running_total;
}
