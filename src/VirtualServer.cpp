#include "VirtualServer.hpp"

void VirtualServer::setHost(const std::string& host)
{
	this->host = host;
}

void VirtualServer::setRootDirectory(const std::string& directory)
{
	this->rootDirectory = directory;
}

void VirtualServer::setErrorPage(int errorCode, const std::string& url)
{
	this->errorPages[errorCode] = url;
}

void VirtualServer::setBodySize(size_t size)
{
	this->bodySize = size;
}

void VirtualServer::setPort(int port)
{
	this->port = port;
}

void VirtualServer::setServerNames(const std::string& serverName)
{
	this->serverNames.push_back(serverName);
}

void VirtualServer::setRoutes(const Route& route)
{
	this->routes.push_back(route);
}

//Constructor
VirtualServer::VirtualServer()
{
	host = "";
	rootDirectory = "";
	bodySize = 0;
	port = 80;
}