#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <dirent.h>

#include "Response.hpp"
#include "Request.hpp"
#include "VirtualServer.hpp"

void	Response::buildDirectoryListing()
{
	std::string h1 = "<h1>Index of" + _rootedPath + "</h1>";

	std::string list, element;

	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (_rootedPath.c_str())) != NULL)
	{
		while ((ent = readdir (dir)) != NULL)
		{
			element.append("<li><a href=\"" + std::string(ent->d_name) + "\"/></li>");
		}
		closedir (dir);
	}
	else
	{
		return perror ("Could not open directory to display directory listing");
	}

	list = "<ul>" + element + "</ul>";
	_body = h1 + list;
}

bool	fileExists(const std::string& path)
{
  struct stat buffer;
  return (stat(path.c_str(), &buffer) == 0); 
}

//prevent Directory traversal attack by removing ../ from path
std::string generatePath(const std::string& root, const std::string& file) {}

//Note use stat to know if path is file or dir
void Response::getRessource()
{
	if (!fileExists(_rootedPath))
	{
		_statusCode = 404;
		return;
	}

	std::ifstream file(_rootedPath, std::ios_base::in);

	if (!file.is_open())
	{
		_statusCode = 500;
		std::cerr << "Error opening ressource" << std::endl;
		return;
	}

	std::stringstream buffer;
    buffer << file.rdbuf();
	_body = buffer.str();

	file.close();
	_contentLength = _body.size();
}

void Response::postRessource(const std::string& path)
{
	//what to do if file already exists? (409 Conflict?)
	//what to do if path is a folder?
}

void	Response::generateStartLine()
{
	std::ostringstream os;
	
	os << "HTTP/1.1 " << _statusCode << " " << statusMessage[_statusCode] << "\r\n";
	_header = os.str();
}

void	Response::build(const VirtualServer& server, const Request &request)
{
	Route *route = NULL;
	bool routeFound = false;

	_statusCode = request.getStatus();

	// std::unordered_map<std::string, Route>::const_iterator it;
	// it = server.getRoutes().find(request.getUrl());
	// if (it != server.getRoutes().end())
	// {
	// 	route = it->second;
	// 	routeFound = true;
	// }

	//here save the rooted path based on location block matching 
	//route = findRoute();

	if (_statusCode == 200)
	{
		if (request.getMethod() == Request::GET)
		{
			//check if path is a directory and do directory stuff, else read the file
			// if (dir && directory_listing)
			// {
				//buildDirectoryListing();
			//}
			//getRessource();
		}
		else if (request.getMethod() == Request::POST)
		{
			//look if rights are ok and create a file then write the content in it, ensure to respect max_body_size of conf

		}
	}

	if (_statusCode != 200)
	{
		std::string errorPage = server.getErrorPage(_statusCode);

		if (!errorPage.empty())
			getRessource(); //here we'll get a problem cause we don't want 404 status if error page is not found

	}

	generateStartLine();
}

Response::Response(): _contentLength(0) {}