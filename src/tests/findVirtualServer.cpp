#include <map>
#include <string>
#include <vector>
#include <iostream>

struct Server
{
	std::string ipPort;
	std::string host;
	size_t port;
	std::vector<std::string> serverNames;
};

struct Request
{
	std::string host;
	std::string ipPort;
};

struct ServerManager
{
	std::multimap<std::string, Server> servers;
};


void findVirtualServer(const std::multimap<std::string, Server>& servers)
{
	Request req;

	req.host = "example.caca";
	req.ipPort = "127.0.0.1:80";

    std::pair<std::multimap<std::string, Server>::const_iterator, std::multimap<std::string, Server>::const_iterator> range;
    range = servers.equal_range(req.ipPort);

	if (range.first == range.second)
	{
		//search for a port matching with all addresses
		std::cout << "no ip:port match, trying *.port match" << std::endl;
		range = servers.equal_range("0.0.0.0:8080");
		if (range.first == range.second)
		{
			//i believe this case is impossible bc we setup our sockets based on the conf file so the request should match,
			//but make sure we handle this just in case
			std::cout << "no port match" << std::endl;
		}
	}

	//filter by server_name (based on Host header field)

	std::multimap<std::string, Server>::const_iterator it = range.first;

	for (; it != range.second; ++it)
	{
		for (std::vector<std::string>::const_iterator serverName = it->second.serverNames.begin(); serverName != it->second.serverNames.end(); ++serverName)
		{
			if (*serverName == req.host)
			{
				std::cout << "Match : " << it->first << ": " << *serverName << std::endl;
				return ;
			}
		}
		std::cout << it->first << " - " << it->second.serverNames.front() << std::endl;
	}

	std::cout << "No match based on server_name so use the first ip:port match " << it->first << " - " << it->second.serverNames.front() << std::endl;
}

int main()
{
	ServerManager manager;
	Server serv, serv2, serv3, serv4, serv5;

	serv.ipPort = "127.0.0.1:80";
	serv.port = 80;
	serv.host = "127.0.0.1";
	serv.serverNames.push_back("example.com");
	serv.serverNames.push_back("example.fr");

	serv2.ipPort = "127.0.0.2:80";
	serv2.port = 80;
	serv2.host = "127.0.0.2";
	serv2.serverNames.push_back("example.com");
	serv2.serverNames.push_back("example.fr");

	serv3.ipPort = "127.0.0.1:80";
	serv3.port = 80;
	serv3.host = "127.0.0.1";
	serv3.serverNames.push_back("example.org");

	serv4.ipPort = "127.0.0.2:8080";
	serv4.port = 8080;
	serv4.host = "127.0.0.1";
	serv4.serverNames.push_back("example.com");
	serv4.serverNames.push_back("example.fr");

	serv5.ipPort = "0.0.0.0:8080";
	serv5.port = 8080;
	serv5.host = "0.0.0.0";
	serv5.serverNames.push_back("example.com");
	serv5.serverNames.push_back("example.fr");

	manager.servers.insert(std::make_pair(serv.ipPort, serv));
	manager.servers.insert(std::make_pair(serv2.ipPort, serv2));
	manager.servers.insert(std::make_pair(serv3.ipPort, serv3));
	manager.servers.insert(std::make_pair(serv4.ipPort, serv4));
	manager.servers.insert(std::make_pair(serv5.ipPort, serv5));

	findVirtualServer(manager.servers);

}


/*
conf

port 80
addr *

port 8080
addr *

port 8080
addr 127.0.0.1

port 8080
addr 127.0.0.1

port 80
addr 127.0.0.1

look for a ip/port pair match
	if not found, look for a port matching for addr that are * (0.0.0.0) if any
if multiple result, try to find a match with server name
if no result use first block that match

*/