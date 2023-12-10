#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

void print_usage()
{
	std::cout << "Usage: <program> [input-file.txt]" << std::endl;
}

struct map_node {
	std::string identifier;
	std::string left_node;
	std::string right_node;
};

std::unordered_map<std::string, struct map_node> node_map;

uint64_t gcd(uint64_t a, uint64_t b)
{
	while (true) 
	{
		if (a == 0) {
			return b;
		}
		b %= a;
		if (b == 0) {
			return a;
		}
		a %= b;
	}	
}

uint64_t lcm(uint64_t a, uint64_t b)
{
	uint64_t t = gcd(a, b);
	return t ? (a / t * b) : 0;
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		print_usage();
		return 1;
	}

	std::string buffer;
	std::string directions;
	std::ifstream stream(argv[1]);

	std::getline(stream, directions);
	std::getline(stream, buffer);

	struct map_node node;
	std::vector<struct map_node> nodes;
	while (std::getline(stream, buffer))
	{
		node.identifier = buffer.substr(0, 3);
		node.left_node = buffer.substr(7, 3);
		node.right_node = buffer.substr(12, 3);
		node_map[node.identifier] = node;
		
		if (node.identifier.at(2) == 'A') {
			nodes.push_back(node);
		}
	}

	uint64_t steps;
	std::vector<uint64_t> node_steps;
	for (struct map_node node : nodes)
	{
		steps = 0;
		while (true)
		{
			if (node.identifier.at(2) == 'Z') {
				node_steps.push_back(steps);
				break;
			}
		
			switch (directions.at(steps % directions.size()))
			{
			case 'L':
				node = node_map.at(node.left_node);
				break;
			case 'R':
				node = node_map.at(node.right_node);
				break;
			default:
				std::cout << "somehow a non L/R character was found" << std::endl;
				exit(1);
				break;
			}

			steps += 1;
		}
	}

	uint64_t minimum_steps = 1;
	for (uint64_t steps : node_steps) {
		minimum_steps = lcm(minimum_steps, steps);
	}
	
	std::cout << "minimum steps: " << minimum_steps << std::endl;
	return 0;
}
