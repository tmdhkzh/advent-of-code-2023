#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

void print_usage()
{
	std::cout << "Usage: <program> [input-file.txt]" << std::endl;
}

enum class HandType
{
	HighCard,
	OnePair,
	TwoPair,
	ThreeOfAKind,
	FullHouse,
	FourOfAKind,
	FiveOfAKind,
};

struct hand {
	std::string hand;
	enum HandType type;
	int wager;
};

struct hand parse_hand(const std::string &input)
{
	std::vector<std::pair<char, int>> card_counts;
	struct hand hand { };
	std::stringstream ss(input);
	ss >> hand.hand >> hand.wager;
	hand.type = HandType::HighCard;

	for (char card : hand.hand)
	{
		auto iterator = std::find_if(card_counts.begin(), card_counts.end(), [&card](const std::pair<char, int> &pair) {
			return pair.first == card;
		});
		if (iterator != card_counts.cend()) {
			iterator->second += 1;
		}
		else {
			card_counts.push_back(std::pair<char, int> { card, 1 });
		}
	}

	for (const auto pair : card_counts)
	{
		if (pair.second == 5) {
			hand.type = HandType::FiveOfAKind;
			break;
		}
		else if (pair.second == 4) {
			hand.type = HandType::FourOfAKind;
			break;
		}
		else if (pair.second == 3) {
			if (hand.type == HandType::OnePair) {
				hand.type = HandType::FullHouse;
				break;
			}
			else {
				hand.type = HandType::ThreeOfAKind;
			}
		}
		else if (pair.second == 2) {
			if (hand.type == HandType::ThreeOfAKind) {
				hand.type = HandType::FullHouse;
				break;
			}
			else if (hand.type == HandType::OnePair) {
				hand.type = HandType::TwoPair;
				break;
			}
			else {
				hand.type = HandType::OnePair;
			}
		}
		else {
			continue;
		}
	}

	return hand;
}

bool is_greater_face(char left, char right)
{
	static const std::unordered_map<char, int> face_map {
		{ 'A', 13 },
		{ 'K', 12 },
		{ 'Q', 11 },
		{ 'J', 10 },
		{ 'T',  9 },
		{ '9',  8 },
		{ '8',  7 },
		{ '7',  6 },
		{ '6',  5 },
		{ '5',  4 },
		{ '4',  3 },
		{ '3',  2 },
		{ '2',  1 },
	};
	return face_map.at(left) > face_map.at(right);
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		print_usage();
		return 1;
	}

	std::string line;
	std::vector<struct hand> hands { };
	std::ifstream stream(argv[1]);
	while (std::getline(stream, line)) {
		hands.push_back(parse_hand(line));
	}

	// sort vector of hands according to their value
	std::sort(hands.begin(), hands.end(), [](const struct hand &left, const struct hand &right) {
		if (left.type == right.type) {
			for (int i = 0; i < 5; ++i) {
				if (left.hand.at(i) == right.hand.at(i)) {
					continue;
				}
				else {
					return is_greater_face(left.hand.at(i), right.hand.at(i));
				}
			}
			// two hands are identical
			return false;
		}
		else {
			return static_cast<int>(left.type) > static_cast<int>(right.type);
		}
	});
	
	uint64_t total = 0;
	for (int i = 0; i < hands.size(); ++i) {
		total += (hands.size() - i ) * hands.at(i).wager;
	}

	std::cout << "total: " << total << std::endl;
	return 0;
}
