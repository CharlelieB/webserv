#include "VirtualServer.hpp"
#include <algorithm>
#include "utils.hpp"
#include <iostream>

std::string normalizePath(const std::string& path)
{
    std::string normalized;
    std::istringstream stream(path);
    std::string segment;
    std::vector<std::string> segments;

    // Diviser le chemin par '/'
    while (std::getline(stream, segment, '/'))
	{
        if (!segment.empty()) {
            segments.push_back(segment);
        }
    }

    // Joindre les segments avec un seul '/'
    for (std::vector<std::string>::const_iterator it = segments.begin(); it != segments.end(); ++it)
	{
        normalized += "/" + *it;
    }

    return normalized.empty() ? "/" : normalized;
}

const Route *VirtualServer::findRoute(const std::string& requestPath) const
{
    std::string normalizedRequestPath = normalizePath(requestPath);
    std::string bestMatch;
    size_t bestMatchSegments = 0;

    // Calculer le nombre de segments dans le chemin de requête
    size_t requestPathSegments = std::count(normalizedRequestPath.begin(), normalizedRequestPath.end(), '/') + 1;

    // Itérer à travers la map pour trouver un chemin correspondant au chemin de requête normalisé
    for (std::map<std::string, Route>::const_iterator it = _routes.begin(); it != _routes.end(); ++it) 
	{
        std::string normalizedRoutePath = normalizePath(it->first);
        std::cout << "noramlized path " << normalizedRoutePath << std::endl;
        // Calculer le nombre de segments dans le chemin de la route
        size_t routePathSegments = std::count(normalizedRoutePath.begin(), normalizedRoutePath.end(), '/') + 1;

        // Ignorer les chemins plus longs que le chemin de requête
        if (routePathSegments > requestPathSegments)
            continue;

        // Vérifier combien de segments correspondent au début du chemin normalisé
        std::istringstream requestStream(normalizedRequestPath);
        std::istringstream routeStream(normalizedRoutePath);
        std::string requestSegment, routeSegment;
        size_t matchSegments = 0;

        while (std::getline(requestStream, requestSegment, '/') && std::getline(routeStream, routeSegment, '/'))
		{
            if (requestSegment == routeSegment) 
                ++matchSegments;
            else 
                break;
        }

        // Mettre à jour la meilleure correspondance si la correspondance actuelle est plus fidèle et n'a pas de segments supplémentaires
        if (matchSegments > bestMatchSegments && matchSegments == routePathSegments)
		{
            bestMatch = it->first;
            bestMatchSegments = matchSegments;
        }
    }

    if (!bestMatch.empty())
		return &_routes.find(bestMatch)->second;
	return NULL;
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