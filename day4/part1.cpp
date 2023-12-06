#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

void print_usage()
{
	std::cout << "Usage: <program> [input-file.txt]" << std::endl;
}

void read_card(const std::string &card, std::vector<int> &card_nums, std::unordered_set<int> &winning_nums)
{
	int num = 0;
	std::string card_nums_segment;
	std::string winning_nums_segment;
	std::stringstream stream(card);
	
	std::getline(stream, card_nums_segment, ':');
	std::getline(stream, card_nums_segment, '|');
	std::getline(stream, winning_nums_segment, '|');

	stream = std::stringstream(card_nums_segment);
	while (stream >> num) {
		card_nums.push_back(num);
	}

	stream = std::stringstream(winning_nums_segment);
	while (stream >> num) {
		winning_nums.insert(num);
	}
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		print_usage();
		return 1;
	}
	
	int local_value;
	int total_value = 0;

	std::string card;
	std::vector<int> card_nums;
	std::unordered_set<int> winning_nums;
	std::ifstream stream(argv[1]);
	while (std::getline(stream, card))
	{
		local_value = 1;
		card_nums.clear();
		winning_nums.clear();

		read_card(card, card_nums, winning_nums);
		for (int num : card_nums) {
			if (winning_nums.find(num) != winning_nums.cend()) {
				local_value <<= 1;
			}
		}
		total_value += (local_value >> 1);
	}

	std::cout << "total_value = " << total_value << std::endl;
	return 0;
}
