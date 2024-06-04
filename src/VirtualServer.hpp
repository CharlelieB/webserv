#pragma once

#include <map>
#include <vector>
#include <string>
#include <map>


#include "Route.hpp"

class VirtualServer
{
	private:
		std::string _host;
		std::string _rootDirectory;
		std::string _ipPort;
		std::map<int, std::string> _errorPages;
		size_t _bodySize;
		int _port;
		std::vector<std::string> _serverNames;
		//std::vector<Route> _routes; //must change to u map with location as key
		std::map<std::string, Route> _routes;
	public:
		VirtualServer();
		void setHost(const std::string& host);
		void setRootDirectory(const std::string& directory);
		void setErrorPage(int errorCode, const std::string& url);
		void setBodySize(size_t size);
		void setPort(int port);
		void setServerNames(const std::string& serverName);
		void setRoutes(const std::string& key, const Route& route);
		void setIpPort();

		int getPort() const;
		std::string getHost() const;
		std::string getIpPort() const;
		std::map<std::string, Route> getRoutes() const;
		std::string	getErrorPage(int status) const;
		std::vector<std::string> getServerNames() const;

		const Route *findRoute(const std::string& requestPath) const;
};
