#include <iostream>

void print_usage()
{
	std::cout << "Usage: <program> [input-file.txt]" << std::endl;
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		print_usage();
		return 1;
	}
	
	return 0;
}
