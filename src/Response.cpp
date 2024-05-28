#include <vector>

#include "Response.hpp"
#include "Request.hpp"
#include "VirtualServer.hpp"


void Response::openFile(const std::string& root, const std::string& file) {}
void Response::createFile(const std::string& root, const std::string& file) {
	//what to do if file already exists? (409 Conflict?)
}



void	Response::build(const VirtualServer& server, const Request &request)
{
	Route route;
	bool routeFound = false;

	_statusCode = request.getStatus();

	std::unordered_map<std::string, Route>::const_iterator it;
	it = server.getRoutes().find(request.getUrl());
	if (it != server.getRoutes().end())
	{
		route = it->second;
		routeFound = true;
	}
	
	if (request.getMethod() == Request::GET)
	{
		// openFile(route.getRoute());
	}
	else if (request.getMethod() == Request::POST)
	{

	}
	//if GET search for the ressource, read it in a buffer (the body) and save the len
		//if ressource does not exist, 404
	//if POST, look if right are ok and create a file then write the content in it, ensure to respect max_body_size of conf

	//if status != 200 check if error page is setup and read the content of the page
}

Response::Response() {}