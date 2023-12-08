#include <algorithm>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <set>
#include <sstream>
#include <string>
#include <vector>

void print_usage()
{
	std::cout << "Usage: <program> [input-file.txt]" << std::endl;
}

struct seed_params {
	uint64_t start;
	uint64_t range;
};

struct almanac_offset {
	uint64_t destination_start;
	uint64_t source_start;
	uint64_t range;
};

void read_almanac_into_vector(std::vector<struct almanac_offset> *vector, std::string &line)
{
	struct almanac_offset almanac_offset;
	std::stringstream ss(line);
	ss >> almanac_offset.destination_start >> almanac_offset.source_start >> almanac_offset.range;
	vector->push_back(almanac_offset);
}

uint64_t reverse_offset_value(uint64_t input, const std::vector<struct almanac_offset> &vector)
{
	std::vector<struct almanac_offset>::const_iterator iterator = std::find_if(vector.cbegin(), vector.cend(),
			[input](const struct almanac_offset &offset) 
	{
		return input >= offset.destination_start && input < offset.destination_start + offset.range;
	});

	if (iterator != vector.cend()) {
		input = iterator->source_start + (input - iterator->destination_start); 
	}
	return input;
}

bool is_valid_seed(uint64_t input, const std::vector<struct seed_params> &seeds)
{
	for (const struct seed_params &param : seeds)
	{
		if (input >= param.start && input < param.start + param.range) {
			return true;
		}
	}
	return false;
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		print_usage();
		return 1;
	}

	std::vector<struct seed_params> seeds { };
	std::vector<struct almanac_offset> seed_to_soil { };
	std::vector<struct almanac_offset> soil_to_fertilizer { };
	std::vector<struct almanac_offset> fertilizer_to_water { };
	std::vector<struct almanac_offset> water_to_light { };
	std::vector<struct almanac_offset> light_to_temperature { };
	std::vector<struct almanac_offset> temperature_to_humidity { };
	std::vector<struct almanac_offset> humidity_to_location { };

	std::ifstream stream(argv[1]);
	std::stringstream ss;
	std::string line;
	std::string fragment;

	// read all seeds
	std::getline(stream, line);
	ss = std::stringstream(line);
	std::getline(ss, fragment, ':');
	while (!ss.eof()) {
		struct seed_params params;
		ss >> params.start >> params.range;
		// add them sorted from highest to lowest
		std::vector<struct seed_params>::iterator iter = seeds.begin();
		while (true) {
			if (iter == seeds.end()) {
				break;
			}
			else if (iter->start < params.start) {
				break;
			}
			else {
				iter ++;
				continue;
			}
		}
		seeds.insert(iter, params);
	}

	// build all of the maps
	std::vector<struct almanac_offset> *current_vector = &seed_to_soil;
	while (std::getline(stream, line))
	{
		if (line.empty()) {
			continue;
		}
		else if (line.at(0) >= 0x30 && line.at(0) <= 0x39) {
			read_almanac_into_vector(current_vector, line);
		}
		else
		{
			if (line.rfind("so", 0) == 0) {
				current_vector = &soil_to_fertilizer;
			}
			else if (line.rfind("fe", 0) == 0) {
				current_vector = &fertilizer_to_water;
			}
			else if (line.rfind("wa", 0) == 0) {
				current_vector = &water_to_light;
			}
			else if (line.rfind("li", 0) == 0) {
				current_vector = &light_to_temperature;
			}
			else if (line.rfind("te", 0) == 0) {
				current_vector = &temperature_to_humidity;
			}
			else if (line.rfind("hu", 0) == 0) {
				current_vector = &humidity_to_location;
			}
		}
	}

	bool found = false;
	int seed_reverse_index = 0;
	struct seed_params current_params;
	uint64_t lowest_location = 0;
	uint64_t current_value = 0;
	while (!found)
	{
		seed_reverse_index += 1;
		if (seed_reverse_index > seeds.size()) {
			std::cout << "not found" << std::endl;
			return 1;
		}

		for (; !found; ++lowest_location)
		{
			current_value = reverse_offset_value(lowest_location, humidity_to_location);
			current_value = reverse_offset_value(current_value, temperature_to_humidity);
			current_value = reverse_offset_value(current_value, light_to_temperature);
			current_value = reverse_offset_value(current_value, water_to_light);
			current_value = reverse_offset_value(current_value, fertilizer_to_water);
			current_value = reverse_offset_value(current_value, soil_to_fertilizer);
			current_value = reverse_offset_value(current_value, seed_to_soil);
			if (is_valid_seed(current_value, seeds)) {
				found = true;
				break;
			}
		}

		if (found) {
			break;
		}
	}
	
	std::cout << "lowest location: " << lowest_location << std::endl;
	return 0;
}
