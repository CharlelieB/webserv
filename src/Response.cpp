#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <dirent.h>

#include "Response.hpp"
#include "Request.hpp"
#include "VirtualServer.hpp"
#include "errorPages.hpp"

std::map<int, std::string> Response::createStatusMessageMap()
{
    std::map<int, std::string> m;
    m[200] = "OK";
    m[301] = "Moved Permanently";
    m[400] = "Bad Request";
	m[403] = "Forbidden";
    m[404] = "Not Found";
    m[405] = "Method Not Allowed";
    m[411] = "Length Required";
    m[413] = "Request Entity Too Large";  // for client_max_body_size
    m[500] = "Internal Server Error";
    m[505] = "HTTP Version Not Supported";
    return m;
}

std::map<std::string, std::string> Response::createContentTypeMap()
{
    std::map<std::string, std::string> m;
	m["mp4"] = "video/mp4";
	m["mp3"] = "audio/mpeg";
	m["html"] = "text/html";
	m["css"] = "text/css";
	m["js"] = "application/javascript";
	m["json"] = "application/json";
	m["xml"] = "application/xml";
	m["png"] = "image/png";
	m["jpg"] = "image/jpeg";
	m["jpeg"] = "image/jpeg";
	m["gif"] = "image/gif";
	m["webp"] = "image/webp";
	m["svg"] = "image/svg+xml";
	m["ico"] = "image/x-icon";
	m["pdf"] = "application/pdf";
	m["zip"] = "application/zip";
	m["tar"] = "application/x-tar";
	m["gz"] = "application/gzip";
	m["txt"] = "text/plain";
	m["csv"] = "text/csv";
	m["mpg"] = "video/mpeg";
	m["mpeg"] = "video/mpeg";
	m["mov"] = "video/quicktime";
	m["avi"] = "video/x-msvideo";
	m["wav"] = "audio/wav";
	m["ogg"] = "audio/ogg";
	m["ogv"] = "video/ogg";
	m["flac"] = "audio/flac";
	m["bmp"] = "image/bmp";
	m["tiff"] = "image/tiff";
	m["tif"] = "image/tiff";
	m["woff"] = "font/woff";
	m["woff2"] = "font/woff2";
	m["ttf"] = "font/ttf";
	m["otf"] = "font/otf";
	m["eot"] = "application/vnd.ms-fontobject";
	m["7z"] = "application/x-7z-compressed";
	m["epub"] = "application/epub+zip";
	m["mobi"] = "application/x-mobipocket-ebook";
	m["doc"] = "application/msword";
	m["docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	m["ppt"] = "application/vnd.ms-powerpoint";
	m["pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	m["xls"] = "application/vnd.ms-excel";
	m["xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	m["rtf"] = "application/rtf";
    return m;
}

// ------WebKitFormBoundary7MA4YWxkTrZu0gW
// Content-Disposition: form-data; name="name"

// John Doe
// ------WebKitFormBoundary7MA4YWxkTrZu0gW
// Content-Disposition: form-data; name="file"; filename="example.txt"
// Content-Type: text/plain

// (File content goes here, binary or text)
// ------WebKitFormBoundary7MA4YWxkTrZu0gW--

// void parseMultipartForm(const std::string &body, const std::string &delimiter)
// {
// 	std::stringstream ss(body);
// 	std::string line;

//     while (std::getline(ss, line) && line != delimiter)
// 	{

// 	}

// }

void Response::normalizePath()
{
    std::vector<std::string> parts;	
    std::stringstream ss(_ressourcePath);
    std::string part;
    
    while (std::getline(ss, part, '/'))
	{
        if (part == "" || part == ".")
            continue;
		else if (part == "..")
		{
            if (!parts.empty())
                parts.pop_back();
        }
		else
            parts.push_back(part);
    }

    std::string normalizedPath = "/";
    for (size_t i = 0; i < parts.size(); ++i)
    {
        normalizedPath += parts[i];
        if (i != parts.size() - 1)
            normalizedPath += "/";
    }

    if (!_ressourcePath.empty() && normalizedPath != "/" && _ressourcePath[_ressourcePath.size() - 1] == '/')
        normalizedPath += "/";

    _ressourcePath = "." + normalizedPath;
}

const std::map<int, std::string>& Response::getStatusMessage()
{
    static const std::map<int, std::string> statusMessage = createStatusMessageMap();
    return statusMessage;
}

const std::map<std::string, std::string>& Response::getContentType()
{
    static const std::map<std::string, std::string> statusMessage = createContentTypeMap();
    return statusMessage;
}

void	Response::addIndex(const std::string &indexFile)
{
    if (_ressourcePath[_ressourcePath.size() - 1] == '/')
        _ressourcePath = _ressourcePath + indexFile;
}

void Response::rootPath(const std::string &root)
{
    std::string fullPath = root + _ressourcePath;

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
			element.append("<li><a href=\"" + std::string(ent->d_name) + "\"/>" + std::string(ent->d_name) + "</li>");
		}
		closedir (dir);
	}
	else
	{	
		return perror ("Could not open directory to display directory listing");
	}

	list = "<ul>" + element + "</ul>";
	_dirListingBody = h1 + list; 
	_body = h1 + list;
}

bool	Response::ressourceExists(const std::string& path)
{
	struct stat buffer;
	bool	fileExists;

	fileExists = stat(path.c_str(), &buffer) == 0;

	// if (fileExists)
  	// 	_pathIsDir = S_ISDIR(buffer.st_mode);
	_contentLength = buffer.st_size;
	std::cout << "request file : " << path << " found " << fileExists << std::endl;
	return fileExists;
}

// void Response::sendBody()
// {

// }

// void Response::getRessource()
// {
// 	std::ifstream file(_ressourcePath.c_str(), std::ios_base::in);

// 	std::cout << _ressourcePath << "-----------------" << std::endl;
// 	if (!file.is_open())
// 	{
// 		_statusCode = 500;
// 		std::cerr << "Error opening ressource" << std::endl;
// 		return;
// 	}

// 	std::stringstream buffer;
//     buffer << file.rdbuf();
// 	_body = buffer.str();
// 	file.close();

// 	_contentLength = _body.size();
// }

void Response::readCustomErrorPage(const std::string& path)
{
	std::ifstream file(path.c_str(), std::ios_base::in);

	if (!file.is_open())
	{
		std::cerr << "Error opening custom error page: " << path << std::endl;
		return;
	}

	std::stringstream buffer;
    buffer << file.rdbuf();
	_body = buffer.str();

	file.close();
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
		<< "server: Webserv\r\n" << "content-length: " << _contentLength << "\r\n" << "content-type:" << _contentType << "\r\n\r\n";
	_header = os.str();
}

void	Response::handleGet()
{
	setContentType();
	// getRessource();
}

bool	Response::isDirectory(const std::string& str)
{
	return str[str.size() - 1] == '/';
}

void	Response::setupRoute(const Route& route)
{
	rootPath(route.getRoot());

	if (_pathIsDir)
	{
		std::string index = route.getIndex();
		if (!index.empty())
		{
			addIndex(index);
			_pathIsDir = false; //index found, not a directory path anymore
		}
	}
}

void Response::setCGIEnv()
{
	std::vector<std::string> env;
    std::string query;
    std::string method;

    if (_method == Methods::POST)
        method = "POST";
    else if (_method == Methods::GET)
        method = "GET";

    env.push_back("REQUEST_METHOD=" + method);
    env.push_back("QUERY_STRING=" + _queryString);
    env.push_back("SCRIPT_NAME=" + _ressourcePath);
    env.push_back("SERVER_PROTOCOL=HTTP/1.1");

	for (std::vector<std::string>::const_iterator it = env.begin(); it != env.end(); ++it)
        _CGIEnv.push_back(const_cast<char*>(it->c_str()));
    _CGIEnv.push_back(NULL);
}

void Response::checkCGI()
{
	if (!_pathIsDir)
	{
		size_t pos = _ressourcePath.find_last_of('.');
		if (pos != std::string::npos)
		{
			_extension = _ressourcePath.substr(pos + 1);
			pos = _extension.find('?');
			if (pos != std::string::npos)
			{
				//get variables after ?
				_queryString = _ressourcePath.substr(pos + 1);
				_extension = _extension.substr(0, pos - 1);
				//remove variables (?name=value) from path
				_ressourcePath = _ressourcePath.substr(0, pos - 1);
			}
			if ((_isCgi = (_extension == "php")))
				setCGIEnv();
		}
	}
}

void Response::setContentType()
{
	if (!_pathIsDir)
	{
		size_t pos = _ressourcePath.find_last_of('.');

		if (pos != std::string::npos)
		{
			checkCGI();
			if (_isCgi)
				return;

			try
			{
				_contentType = getContentType().at(_extension);
			}
			catch (const std::out_of_range& oor)
			{
				_contentType = "application/octet-stream";
			}
		}
		else
			_contentType = "application/octet-stream";
	}
}

void Response::manageRouting(const Route* route)
{
	_pathIsDir = isDirectory(_ressourcePath);
	if (route)
	{
		if (!route->isMethodAllowed(_method))
		{
			_statusCode = 405;
			return;
		}
		setupRoute(*route);
	}
	else
	{
		rootPath("www");
	}

	normalizePath();

	if (!ressourceExists(_ressourcePath))
		_statusCode = 404;
}

void Response::handleRequestByMethod(const Route *route)
{
	if (_method == Methods::GET)
	{
		if (_pathIsDir)
		{
			if (route && route->getAutoIndex())
				buildDirectoryListing();
			else
				_statusCode = 403;
		}
		else
			handleGet();
	}
	else if (_method == Methods::POST)
	{
		checkCGI();
		//look if rights are ok and create a file then write the content in it, ensure to respect max_body_size of conf
	}
}

void	Response::build(const VirtualServer& server, const Request &request)
{
	_method = request.getMethod();
	_statusCode = request.getStatus();
	_ressourcePath = request.getUrl();
std::cout << "ressource path " << _ressourcePath << std::endl;

	const Route *route = server.findRoute(_ressourcePath);

	manageRouting(route);

std::cout << "DEBUG - Request path : " << _ressourcePath << std::endl;
	if (_statusCode == 200)
	{
		handleRequestByMethod(route);
	}

	if (_statusCode != 200)
	{
		std::string errorPage = server.getErrorPage(_statusCode);

		if (!errorPage.empty())
			readCustomErrorPage("www/" + errorPage);
		else
			_body = ErrorPages::getDefaultErrorPage(_statusCode, getStatusMessage().at(_statusCode)); //could be dangerous if map return map.end()
		_contentLength = _body.size();
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
	_isCgi = false;
	_queryString.clear();
	_extension.clear();
	_CGIEnv.clear();
}

// std::string Response::getContent() const
// {
// 	return _header + _body;
// }

std::string Response::getBody() const
{
	return _body;
}

std::string Response::getHeader() const
{
	return _header;
}

size_t Response::getContentLen() const { return _contentLength; }
std::string Response::getPath() const { return _ressourcePath; }

int Response::getStatus() const { return _statusCode ; }
bool Response::getIsCGI() const { return _isCgi ; }
std::vector<char *> Response::getCGIEnv() const { return _CGIEnv; }

Response::Response(): _contentType("text/html"), _contentLength(0), _pathIsDir(false), _isCgi(false) {}