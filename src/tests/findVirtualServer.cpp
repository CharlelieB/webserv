#include <map>
#include <string>
#include <vector>

struct Server
{
	std::string ipPort;
	std::string host;
	size_t port;
	std::vector<std::string> serverNames;
};

struct ServerManager
{
	std::multimap<std::string, Server> servers;
};


int main()
{
	ServerManager manager;
	Server serv, serv2, serv3, serv4;

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

	manager.servers.insert(std::make_pair(serv.ipPort, serv));





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