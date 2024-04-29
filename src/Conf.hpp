#pragma once

#include <map>
#include <vector>
#include <string>

enum methods
{
	POST,
	GET,
	DELETE
};

struct acceptedMethod
{
    enum methods method;
    bool accepted;
};

struct Route
{
	std::string location;
	acceptedMethod methods[3];
	std::vector<std::string> redirections;
	bool directoryListing;
	std::string defaultFile;
	std::string root;
};

struct VirtualServer
{
	std::string host;
	int port;
	std::string serverName;
	std::string rootDirectory;
	std::map<int, std::string> errorPages;
	size_t bodySize;
	std::vector<Route> routes;
};

class Configuration
{
	public:
		void init(const char *filename);
	private:
    	std::vector<VirtualServer> servers;
};