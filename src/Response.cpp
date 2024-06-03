#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <dirent.h>

#include "Response.hpp"
#include "Request.hpp"
#include "VirtualServer.hpp"

std::map<int, std::string> Response::createStatusMessageMap()
{
    std::map<int, std::string> m;
    m[200] = "OK";
    m[301] = "Moved Permanently";
    m[400] = "Bad Request";
    m[404] = "Not Found";
    m[405] = "Method Not Allowed";
    m[411] = "Length Required";
    m[413] = "Request Entity Too Large";  // for client_max_body_size
    m[500] = "Internal Server Error";
    m[505] = "HTTP Version Not Supported";
    return m;
}

const std::map<int, std::string>& Response::getStatusMessage()
{
    static const std::map<int, std::string> statusMessage = createStatusMessageMap();
    return statusMessage;
}

void	Response::addIndex(const std::string &indexFile)
{
    if (_ressourcePath[_ressourcePath.size() - 1] == '/')
        _ressourcePath = _ressourcePath + indexFile;
}

void Response::rootPath(const std::string &root, const std::string &baseUrl)
{
    std::string baseUrlWithSlash = baseUrl;
    if (baseUrlWithSlash[baseUrlWithSlash.size() - 1] != '/')
        baseUrlWithSlash += "/";

    std::string rootWithSlash = root;
    if (rootWithSlash[rootWithSlash.size() - 1] != '/')
        rootWithSlash += "/";

    std::string relativeUrl;
    if (_ressourcePath.find(baseUrlWithSlash) == 0)
        relativeUrl = _ressourcePath.substr(baseUrlWithSlash.size());
    else
        relativeUrl = _ressourcePath.substr(baseUrl.size());

    std::string fullPath = rootWithSlash + relativeUrl;

    _ressourcePath = fullPath;
}

void	Response::buildDirectoryListing()
{
	std::string h1 = "<h1>Index of" + _ressourcePath + "</h1>";

	std::string list, element;

	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (_ressourcePath.c_str())) != NULL)
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

bool	Response::checkFile(const std::string& path)
{
	struct stat buffer;
	bool	fileExists;

	fileExists = stat(path.c_str(), &buffer) == 0;

	if (fileExists)
  		_pathIsDir = S_ISDIR(buffer.st_mode);
	return fileExists;
}

void Response::getRessource()
{
	std::ifstream file(_ressourcePath.c_str(), std::ios_base::in);

	std::cout << _ressourcePath << "-----------------" << std::endl;
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

// void Response::postRessource(const std::string& path)
// {
// 	//what to do if file already exists? (409 Conflict?)
// 	//what to do if path is a folder?
// }

void	Response::generateHeader()
{
	std::ostringstream os;
	
	os << "HTTP/1.1 " << _statusCode << " " << getStatusMessage().at(_statusCode) << "\r\n"
		<< "content-length:" << _body.size() << "\r\n\r\n";
	_header = os.str();
}

void	Response::handleGet()
{
	if (_route && _pathIsDir)
	{
		std::string index = _route->getIndex();
		if (!index.empty())
			addIndex(index);
		else if (_route->getAutoIndex())
			buildDirectoryListing();
		// else
		// 	return;
	}
	getRessource();
}

// void	Response::checkLocationRules()
// {
// 	if (!_route->isMethodAllowed(request.getMethod())))
// 	{
// 		_statusCode = 405;
// 		return;
// 	}
// }

void	Response::build(const VirtualServer& server, const Request &request)
{
	_statusCode = request.getStatus();
	_ressourcePath = request.getUrl();
	_route = server.findRoute(_ressourcePath);

	if (_route)
	{
		rootPath(_route->getRoot(), _route->getLocation());
		//checkLocationRules();
	}
	else
		rootPath("www", "/");

	if (!checkFile(_ressourcePath))
		_statusCode = 404;

	if (_statusCode == 200)
	{
		if (request.getMethod() == Methods::GET)
			handleGet();
		else if (request.getMethod() == Methods::POST)
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
	generateHeader();
}

void	Response::reset()
{
	_header.clear();
	_body.clear();
	_ressourcePath.clear();
	_contentLength = 0;
	_pathIsDir = false;
	_route = NULL; 
}

std::string Response::getContent() const { return _header + _body; }

Response::Response(): _contentLength(0), _pathIsDir(false), _route(NULL) {}