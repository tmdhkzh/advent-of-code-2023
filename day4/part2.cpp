#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
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
	
	std::ifstream stream(argv[1]);
	int num_cards = 0;
	int local_value;
	int total_count = 0;

	std::string card;
	std::vector<int> card_nums;
	std::unordered_set<int> winning_nums;
	std::unordered_map<int, int> card_copies;
	while (std::getline(stream, card))
	{
		num_cards += 1;
		if (card_copies.find(num_cards) == card_copies.end()) {
			card_copies[num_cards] = 1;
		}

		local_value = 0;
		card_nums.clear();
		winning_nums.clear();

		read_card(card, card_nums, winning_nums);
		for (int num : card_nums) {
			if (winning_nums.find(num) != winning_nums.cend()) {
				local_value += 1;
			}
		}

		int target_index;
		for (int i = 0; i < local_value; ++i) {
			target_index = num_cards + i + 1;
			if (card_copies.find(target_index) == card_copies.end()) {
				card_copies[target_index] = 1;
			}
			card_copies.at(target_index) += card_copies.at(num_cards);
		}
	}

	for (int i = 1; i <= num_cards; ++i) {
		total_count += card_copies.at(i);
	}

	std::cout << "total_count = " << total_count << std::endl;
	return 0;
}
