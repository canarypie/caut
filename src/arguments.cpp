#include "arguments.h"
#include "utility.h"
#include <iostream>

Config Config::handle_arguments(const int count, char** arguments)
{
	Config config;

	int opt;
	while ((opt = getopt(count, arguments, "p:f:s:r:c:N:R:vh")) != -1)
	{
		switch (opt)
		{
			case 'p':
			{
				config.pattern = optarg;
				break;
			}
			case 'f':
			{
				config.file = optarg;
				break;
			}
			case 'v':
				std::cout << "ConwayGOL version " << GOL_VERSION << '\n'; 
				exit(0);
			case 'h':
				print_usage();
				exit(0);
			case 's':
			{
				config.simulations = std::stoi(optarg);
				break;
			}
			case 'r': 
			{
				config.rows = std::stoi(optarg);
				break;
			}
			case 'c':
			{
				config.cols = std::stoi(optarg);
				break;
			}
			case 'N':
			{
				config.neighbors_type = optarg;
				break;
			}
			case 'R':
			{
				config.rule = optarg;
				break;
			}
			default:
			{
				std::cout << "Unknown argument\n";
			}
		}
	}
	return config;
}

