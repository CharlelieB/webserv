#include <iostream>
#include "Conf.hpp"

int	main(int argc, char **argv)
{
	Configuration conf;

	if (argc != 2)
	{
		std::cout << "Wrong arguments" << std::endl;
		return 1;
	}

	conf.init(argv[1]);
}