#include <algorithm>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

void print_usage()
{
	std::cout << "Usage: <program> [input-file.txt]" << std::endl;
}

struct almanac_offset {
	uint64_t destination_start;
	uint64_t source_start;
	uint64_t range;
};

void read_values_into_vector(std::vector<struct almanac_offset> *vector, std::string &line)
{
	struct almanac_offset almanac_offset;
	std::stringstream ss(line);
	ss >> almanac_offset.destination_start >> almanac_offset.source_start >> almanac_offset.range;
	vector->push_back(almanac_offset);
}

uint64_t get_offset_value(uint64_t input, const std::vector<struct almanac_offset> &vector)
{
	std::vector<struct almanac_offset>::const_iterator iterator = std::find_if(vector.cbegin(), vector.cend(),
			[input](const struct almanac_offset &offset) 
	{
		return input >= offset.source_start && input < offset.source_start + offset.range;
	});

	if (iterator != vector.cend()) {
		input = iterator->destination_start + (input - iterator->source_start); 
	}
	return input;
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		print_usage();
		return 1;
	}

	std::vector<uint64_t> seeds { };
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
	while (std::getline(ss, fragment, ' ')) {
		if (fragment.empty()) {
			continue;
		}
		seeds.push_back(std::stoull(fragment.c_str()));
	}

	// build all of the maps
	std::vector<struct almanac_offset> *current_vector = &seed_to_soil;
	while (std::getline(stream, line))
	{
		if (line.empty()) {
			continue;
		}
		else if (line.at(0) >= 0x30 && line.at(0) <= 0x39) {
			read_values_into_vector(current_vector, line);
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

	uint64_t lowest_location = std::numeric_limits<uint64_t>::max();
	for (uint64_t seed : seeds)
	{	
		seed = get_offset_value(seed, std::ref(seed_to_soil));
		seed = get_offset_value(seed, std::ref(soil_to_fertilizer));
		seed = get_offset_value(seed, std::ref(fertilizer_to_water));
		seed = get_offset_value(seed, std::ref(water_to_light));
		seed = get_offset_value(seed, std::ref(light_to_temperature));
		seed = get_offset_value(seed, std::ref(temperature_to_humidity));
		seed = get_offset_value(seed, std::ref(humidity_to_location));
		lowest_location = (seed < lowest_location) ? seed : lowest_location;
	}
	
	std::cout << "lowest location: " << lowest_location << std::endl;
	return 0;
}
