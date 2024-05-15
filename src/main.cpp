#include <iostream>
#include "Conf.hpp"

int	main(int argc, char **argv)
{

	if (argc != 2)
	{
		std::cout << "Wrong arguments" << std::endl;
		return 1;
	}

	Configuration config(argv[1]);

	try
	{
		config.parseConf();
	}
	catch(std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}
	for (std::vector<VirtualServer>::const_iterator it = config.getServers().begin(); it != config.getServers().end(); ++it)
	{
		std::cout << it->getPort() << std::endl;
		std::cout << it->getHost() << std::endl;
	}
}