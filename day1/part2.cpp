#include <fstream>
#include <iostream>
#include <unordered_map>

void print_usage()
{
	std::cout << "Usage: main [path_to_input]" << std::endl;
}

inline bool is_digit(const std::string::const_iterator &iter)
{
	return *iter >= 0x30 && *iter <= 0x39;
}

inline std::pair<bool, int> is_num_string(
		const std::string::const_iterator &start,
	 	const std::string::const_iterator &end)
{
	static const std::unordered_map<std::string, int> num_map {
		{ "zero" , 0 },
		{ "one"  , 1 },
		{ "two"  , 2 },
		{ "three", 3 },
		{ "four" , 4 },
		{ "five" , 5 },
		{ "six"  , 6 },
		{ "seven", 7 },
		{ "eight", 8 },
		{ "nine" , 9 },
	};

	if (end - start < 3) {
		return { false, 0 };
	}

	std::string str;
	str.push_back(*(start + 0));
	str.push_back(*(start + 1));
	str.push_back(*(start + 2));
	if (num_map.find(str) != num_map.end()) {
		return { true, num_map.at(str) };
	}

	for (int i = 0; i < 2; ++i)
	{
		if (end - start < 4 + i) {
			return { false, 0 };
		}
		
		str.push_back(*(start + 3 + i));
		if (num_map.find(str) != num_map.end()) {
			return { true, num_map.at(str) };
		}
	}
	
	return { false, 0 };
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
	std::pair<bool, int> num_string_result;
	while (std::getline(stream, line))
	{
		// get first digit
		for (std::string::const_iterator iter = line.cbegin(); iter < line.cend(); ++iter) {
			if (is_digit(iter)) {
				running_total += (*iter - 0x30) * 10;
				break;
			}
			else {
				num_string_result = is_num_string(iter, line.cend());
				if (num_string_result.first) {
					running_total += num_string_result.second * 10;
					break;
				}
			}
		}
		for (std::string::const_iterator iter = line.cend() - 1; iter >= line.cbegin(); --iter) {
			if (is_digit(iter)) {
				running_total += *iter - 0x30;
				break;
			}
			else {
				num_string_result = is_num_string(iter, line.cend());
				if (num_string_result.first) {
					running_total += num_string_result.second;
					break;
				}
			}
		}
	}

	std::cout << "total = " << running_total << std::endl;

	return 0;
}
