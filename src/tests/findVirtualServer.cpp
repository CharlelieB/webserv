#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "../utils.hpp"

struct VirtualServer
{
	std::string _ipPort;
	std::string _host;
	size_t _port;
	std::vector<std::string> _serverNames;
};

struct ServerManager
{
	std::multimap<std::string, VirtualServer> servers;
};

struct Request
{
	std::string host;
	std::string listen;
	size_t port;
};

void findVirtualServer(const std::multimap<std::string, VirtualServer>& servers, const Request& req)
{
	std::string host = req.host;

	//add default port to host header if no port
	if (host.find(":") == std::string::npos)
		host += ":80";

    std::pair<std::multimap<std::string, VirtualServer>::const_iterator, std::multimap<std::string, VirtualServer>::const_iterator> range;
    range = servers.equal_range(req.listen);

	if (range.first == range.second)
	{
		//search for a port matching with all addresses
		std::cout << "no ip:port match, trying *.port match" << std::endl;

    	std::string portStr = Utils::nbToStr(req.port);
 
		range = servers.equal_range("0.0.0.0:" + portStr);
		if (range.first == range.second)
		{
			//i believe this case is impossible bc we setup our sockets based on the conf file so the request should match,
			//but make sure we handle this just in case
			std::cout << "no port match" << std::endl;
		}
	}

	//filter by server_name (based on Host header field)

	std::multimap<std::string, VirtualServer>::const_iterator it = range.first;
	std::multimap<std::string, VirtualServer>::const_iterator begin = it;
	std::string port;

	for (; it != range.second; ++it)
	{
		port = Utils::nbToStr(it->second._port);
		for (std::vector<std::string>::const_iterator serverName = it->second._serverNames.begin(); serverName != it->second._serverNames.end(); ++serverName)
		{
			if ((*serverName + ":" + port) == host)
			{
				std::cout << "Match : " << it->first << ": " << *serverName << std::endl;
				return ;
			}
		}
		//std::cout << it->first << " - " << it->second.serverNames.front() << std::endl;
	}

	std::cout << "No match based on server_name, the first ip:port match is " << begin->first << " - " << begin->second._serverNames.front() << std::endl;
}

int main()
{
	//THINK ABOUT SERVER NAME FORMAT (server:port)
	ServerManager manager;
	VirtualServer serv, serv2, serv3, serv4, serv5;

	serv._ipPort = "127.0.0.1:80";
	serv._port = 80;
	serv._host = "127.0.0.1";
	serv._serverNames.push_back("example.com");
	serv._serverNames.push_back("example.fr");

	serv2._ipPort = "127.0.0.2:80";
	serv2._port = 80;
	serv2._host = "127.0.0.2";
	serv2._serverNames.push_back("example.com");
	serv2._serverNames.push_back("example.fr");

	serv3._ipPort = "127.0.0.1:80";
	serv3._port = 80;
	serv3._host = "127.0.0.1";
	serv3._serverNames.push_back("example.org");

	serv4._ipPort = "127.0.0.2:8080";
	serv4._port = 8080;
	serv4._host = "127.0.0.1";
	serv4._serverNames.push_back("example.com");
	serv4._serverNames.push_back("example.fr");

	serv5._ipPort = "0.0.0.0:8080";
	serv5._port = 8080;
	serv5._host = "0.0.0.0";
	serv5._serverNames.push_back("example.com");
	serv5._serverNames.push_back("example.fr");

	manager.servers.insert(std::make_pair(serv._ipPort, serv));
	manager.servers.insert(std::make_pair(serv2._ipPort, serv2));
	manager.servers.insert(std::make_pair(serv3._ipPort, serv3));
	manager.servers.insert(std::make_pair(serv4._ipPort, serv4));
	manager.servers.insert(std::make_pair(serv5._ipPort, serv5));

	Request req;
	std::cout << "[TEST] Server_name match :" << std::endl;
	req.host = "example.org";
	req.listen = "127.0.0.1:80";
	req.port = 80;

	findVirtualServer(manager.servers, req);

	std::cout << "[TEST] No server_name match :" << std::endl;
	req.host = "example.io";
	req.listen = "127.0.0.1:80";
	req.port = 80;

	findVirtualServer(manager.servers, req);

	std::cout << "[TEST] No ip match, find block with *:port match :" << std::endl;
	req.host = "example.fr:8080";
	req.listen = "148.0.1.1:8080";
	req.port = 8080;

	findVirtualServer(manager.servers, req);
}