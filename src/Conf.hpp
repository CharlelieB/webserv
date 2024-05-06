#pragma once

#include <map>
#include <vector>
#include <string>

struct Route
{
	std::string location;
	std::map<std::string, bool> methods;
	std::string redirections;
	bool directoryListing;
	std::string defaultFile;
	std::string root;
	std::string index;
	std::string cgiPath;
	bool autoIndex;
};

struct VirtualServer
{
	VirtualServer();
	std::string host;
	std::string rootDirectory;
	std::map<int, std::string> errorPages;
	size_t bodySize;
	int port;
	std::vector<std::string> serverNames;
	std::vector<Route> routes;
};

class Configuration
{
	public:
		void init(const char *filename);
		void setServers(VirtualServer server);
		const std::vector<VirtualServer>& getServers() const;
		void setRoutes(Route route);
	private:
    	std::vector<VirtualServer> servers;
};