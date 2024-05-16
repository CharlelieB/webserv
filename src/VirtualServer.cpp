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

void VirtualServer::setRoutes(const Route& route)
{
	this->_routes.push_back(route);
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


//Constructor
VirtualServer::VirtualServer(): _host("0.0.0.0"), _bodySize(0), _port(80) {}