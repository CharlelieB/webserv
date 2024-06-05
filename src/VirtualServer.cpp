#include "VirtualServer.hpp"
#include <algorithm>
#include "utils.hpp"
#include <iostream>

// std::string normalizePath(const std::string& path)
// {
//     std::string normalized;
//     std::istringstream stream(path);
//     std::string segment;
//     std::vector<std::string> segments;

//     // Diviser le chemin par '/'
//     while (std::getline(stream, segment, '/'))
// 	{
//         if (!segment.empty()) {
//             segments.push_back(segment);
//         }
//     }

//     // Joindre les segments avec un seul '/'
//     for (std::vector<std::string>::const_iterator it = segments.begin(); it != segments.end(); ++it)
// 	{
//         normalized += "/" + *it;
//     }

//     return normalized.empty() ? "/" : normalized;
// }


const Route *VirtualServer::findRoute(const std::string& requestPath) const
{
	std::string match;
	std::string location;
	size_t locationLen = 0;
	size_t longest = 0;

	for (std::map<std::string, Route>::const_iterator it = _routes.begin(); it != _routes.end(); ++it)
	{
		location = it->first;
		locationLen = location.size();
		if (Utils::compareFirstNChar(location, requestPath, locationLen) && longest < locationLen)
		{
			match = location;
			longest = locationLen;
		}
	}
	return match.empty() ? NULL : &_routes.find(match)->second; 
}

void VirtualServer::setHost(const std::string& host)
{
	this->_host = host;
}

void VirtualServer::setRootDirectory(const std::string& directory)
{
	this->_rootDirectory = directory;
}

void VirtualServer::setErrorPage(int errorCode, const std::string& url)
{
	this->_errorPages[errorCode] = url;
}

void VirtualServer::setBodySize(size_t size)
{
	this->_bodySize = size;
}

void VirtualServer::setPort(int port)
{
	this->_port = port;
}

void VirtualServer::setServerNames(const std::string& serverName)
{
	this->_serverNames.push_back(serverName);
}

void VirtualServer::setRoutes(const std::string& key, const Route& route)
{
	this->_routes[key] = route;
}

void VirtualServer::setIpPort()
{
	_ipPort = _host + ":" + Utils::nbToStr(_port);
}

//Getters

int VirtualServer::getPort() const
{
	return this->_port;
}

std::string VirtualServer::getHost() const
{
	return this->_host;
}

std::string VirtualServer::getIpPort() const
{
	return this->_ipPort;
}

std::map<std::string, Route> VirtualServer::getRoutes() const
{
	return _routes;
}

std::string VirtualServer::getErrorPage(int status) const
{
	std::string page = "";

	std::map<int, std::string>::const_iterator it = _errorPages.find(status);
	
	if (it != _errorPages.end())
		page = it->second;
	
	return page;
}

std::vector<std::string> VirtualServer::getServerNames() const
{
    return _serverNames;
}

//Constructor
VirtualServer::VirtualServer(): _host("0.0.0.0"), _bodySize(1000000), _port(80) {}