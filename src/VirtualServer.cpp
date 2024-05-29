#include "VirtualServer.hpp"

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

//Getters

int VirtualServer::getPort() const
{
	return this->_port;
}

std::string VirtualServer::getHost() const
{
	return this->_host;
}

std::unordered_map<std::string, Route> VirtualServer::getRoutes() const
{
	return _routes;
}

std::string VirtualServer::getErrorPage(int status) const
{
	std::string page = "";

	std::map<int, std::string>::const_iterator it = _errorPages.find(status);
	
	if (it != _errorPages.end())
		page = it->first;
	
	return page;
}

//Constructor
VirtualServer::VirtualServer(): _host("0.0.0.0"), _bodySize(1000000), _port(80) {}