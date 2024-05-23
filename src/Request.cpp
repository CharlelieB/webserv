#include "Request.hpp"

#include <iostream>
#include <sstream>
#include <vector>

void    Request::setMethod(enum eMethods method)
{
    _method = method;
}

void    Request::setUrl(const std::string& url)
{
    _url = url;
}

// void    Request::setHttpVersion(const std::string& version)
// {
//     _httpVersion = version;
// }

void	Request::addHeader(const std::string& key, const std::string& value)
{
    _headers[key] = value;
}

// std::string trim(const std::string &str)
// {
//     size_t first = str.find_first_not_of(" \t\r\n");
//     size_t last = str.find_last_not_of(" \t\r\n");
//     return (first == std::string::npos || last == std::string::npos) ? "" : str.substr(first, last - first + 1);
// }

// std::vector<std::string> split(const std::string &str, char delimiter)
// {
//     std::vector<std::string> tokens;
//     std::string token;
//     std::istringstream tokenStream(str);

//     while (std::getline(tokenStream, token, delimiter))
// 	{
//         tokens.push_back(token);
//     }
//     return tokens;
// }

// void Request::parse(const std::string& data)
// {
// 	std::istringstream requestStream(data);
//     std::string line;

//     // Parse the request line
//     std::getline(requestStream, line);
//     std::vector<std::string> requestLine = split(trim(line), ' ');

//     if (requestLine.size() != 3)
// 	{
//         std::cerr << "Invalid HTTP request line" << std::endl;
//         return;
//     }

//     _method = requestLine[0];
//     _url = requestLine[1];
//     _httpVersion = requestLine[2];

//     // Parse headers
//     while (std::getline(requestStream, line) && line != "\r")
// 	{
//         size_t pos = line.find(":");
//         if (pos != std::string::npos)
// 		{
//             std::string headerName = trim(line.substr(0, pos));
//             std::string headerValue = trim(line.substr(pos + 1));
//             _headers[headerName] = headerValue;
//         }
//     }

//     //IF THERE IS A BODY MUST USE CONTENT-LENGTH FIELD (orTransfer-Encoding: chunked)
//     //IF NO FIELD BUT BODY, BAD REQUEST

//     // Parse body (if any)
// 	if (std::getline(requestStream, _body))
// 	{
//         _body = trim(_body);
//     }
// }

std::map<std::string, std::string> Request::getHeaders() const
{
	return _headers;
}