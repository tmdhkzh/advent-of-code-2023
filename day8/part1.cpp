#include <fstream>
#include <iostream>
#include <unordered_map>

void print_usage()
{
	std::cout << "Usage: <program> [input-file.txt]" << std::endl;
}

struct map_node {
	std::string identifier;
	std::string left_node;
	std::string right_node;
};

int main(int argc, char **argv)
{
	if (argc != 2) {
		print_usage();
		return 1;
	}

	std::string buffer;
	std::string directions;
	std::ifstream stream(argv[1]);
	std::unordered_map<std::string, struct map_node> map;

	std::getline(stream, directions);
	std::getline(stream, buffer);

	struct map_node node;
	while (std::getline(stream, buffer))
	{
		node.identifier = buffer.substr(0, 3);
		node.left_node = buffer.substr(7, 3);
		node.right_node = buffer.substr(12, 3);
		map[node.identifier] = node;
	}

	int steps = 0;
	node = map.at("AAA");
	while (true)
	{
		if (node.identifier == "ZZZ") {
			break;
		}
		
		switch (directions.at(steps % directions.size()))
		{
		case 'L':
			node = map.at(node.left_node);
			break;
		case 'R':
			node = map.at(node.right_node);
			break;
		default:
			std::cout << "somehow a non L/R character was found" << std::endl;
			exit(1);
			break;
		}

		steps += 1;
	}

	std::cout << "total steps: " << steps << std::endl;
	return 0;
}
