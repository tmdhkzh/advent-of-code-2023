#include <fstream>
#include <iostream>

void print_usage()
{
	std::cout << "Usage: main [path_to_input]" << std::endl;
}

constexpr bool is_digit(char c)
{
	return c >= 0x30 && c <= 0x39;
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		print_usage();
		return 1;
	}

	int running_total = 0;

	std::string line;
	std::ifstream stream(argv[1]);
	while (std::getline(stream, line))
	{
		// get first digit
		for (int i = 0; i < line.length(); ++i) {
			if (is_digit(line.at(i))) {
				running_total += (line.at(i) - 0x30) * 10;
				break;
			} 
		}
		// get second digit
		for (int i = line.length() - 1; i >= 0; --i) {
			if (is_digit(line.at(i))) {
				running_total += line.at(i) - 0x30; 
				break;
			}
		}
	}

	std::cout << "total = " << running_total << std::endl;

	return 0;
}
