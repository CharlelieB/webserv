#pragma once

#include <map>
#include <vector>
#include <string>

#include "Route.hpp"

class VirtualServer
{
	private:
		std::string host;
		std::string rootDirectory;
		std::map<int, std::string> errorPages;
		size_t bodySize;
		int port;
		std::vector<std::string> serverNames;
		std::vector<Route> routes;
	public:
		VirtualServer();
		void setHost(const std::string& host);
		void setRootDirectory(const std::string& directory);
		void setErrorPage(int errorCode, const std::string& url);
		void setBodySize(size_t size);
		void setPort(int port);
		void setServerNames(const std::string& serverName);
		void setRoutes(const Route& route);
};
