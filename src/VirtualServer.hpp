#pragma once

#include <map>
#include <vector>
#include <string>

#include "Route.hpp"

class VirtualServer
{
	private:
		std::string _host;
		std::string _rootDirectory;
		std::map<int, std::string> _errorPages;
		size_t _bodySize;
		int _port;
		std::vector<std::string> _serverNames;
		std::vector<Route> _routes;
	public:
		VirtualServer();
		void setHost(const std::string& host);
		void setRootDirectory(const std::string& directory);
		void setErrorPage(int errorCode, const std::string& url);
		void setBodySize(size_t size);
		void setPort(int port);
		void setServerNames(const std::string& serverName);
		void setRoutes(const Route& route);

		int getPort() const;
		std::string getHost() const;

		// bool operator<(const VirtualServer& other) const
		// {
		// 	if (_host != other._host)
		// 		return _host < other._host;
        // 	return _port < other._port;
		// }
    	// bool operator==(const VirtualServer& other) const
		// {
        // 	return _host == other._host && _port == other._port;
    	// }
};
