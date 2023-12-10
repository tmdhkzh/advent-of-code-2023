#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void print_usage()
{
	std::cout << "Usage: <program> [input-file.txt]" << std::endl;
}

bool is_all_zeroes(const std::vector<uint64_t> &vector)
{
	for (uint64_t val : vector) {
		if (val != 0) {
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

	int index;	
	uint64_t current_value = 0;
	uint64_t total_value = 0;
	std::vector<uint64_t> nums;
	std::string report;
	std::ifstream stream(argv[1]);
	std::stringstream report_stream;
	while (std::getline(stream, report))
	{
		nums.clear();
		report_stream = std::stringstream(report);
		while (!report_stream.eof()) {
			report_stream >> current_value;
			nums.push_back(current_value);
		}

		index = 0;
		current_value = nums.at(nums.size() - 1);
		while (true)
		{
			if (is_all_zeroes(nums)) {
				break;
			}

			current_value += nums.at(nums.size() - 1) - nums.at(nums.size() - 2);
			for (int i = 0; i < nums.size() - 1; ++i) {
				nums.at(i) = (nums.at(i + 1) - nums.at(i));
			}
			index += 1;
			nums.pop_back();
		}
		total_value += current_value;
	}

	std::cout << "total value = " << total_value << std::endl;
	return 0;
}
