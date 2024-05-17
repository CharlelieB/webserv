#include <iostream>
#include "Conf.hpp"
#include "Request.hpp"
#include "ServerManager.hpp"

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
		ServerManager server(config.getServers());
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

	// Request req;

	// std::string str = "GET /index.html HTTP/1.1\r\n"
    //     "Host: www.example.com\r\n"
    //     "Connection: keep-alive\r\n"
    //     "User-Agent: Mozilla/5.0\r\n"
    //     "Accept: text/html\r\n"
    //     "\r\n"
    //     "This is the body.";

	// req.parse(str);
	// std::map<std::string, std::string> head = req.getHeaders();
	// std::map<std::string, std::string>::const_iterator it = head.find("Host");
	// std::cout << it->second << std::endl;
}