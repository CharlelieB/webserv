#include <iostream>
#include "Conf.hpp"
#include "Request.hpp"
#include "ServerManager.hpp"
#include <signal.h>
#include <unistd.h>

// Ignore SIGPIPE signal
void ignore_sigpipe() {
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGPIPE, &sa, NULL);
}


int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Wrong arguments" << std::endl;
		return 1;
	}

	ignore_sigpipe();

	Configuration config(argv[1]);

	try
	{
		config.parseConf();
		ServerManager server(config.getServers());
		server.run();
		//server.setAddressesToListen();

	}
	catch(std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}
	// for (std::vector<VirtualServer>::const_iterator it = config.getServers().begin(); it != config.getServers().end(); ++it)
	// {
	// 	std::cout << it->getPort() << std::endl;
	// 	std::cout << it->getHost() << std::endl;
	// }
}