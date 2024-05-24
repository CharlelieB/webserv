#include "Request.hpp"

#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib>
int Request::parseBodyLength(const std::string& str)
{
    char* p;
    long n = strtol(str.c_str(), &p, 10);
    
    if (*p)
        return -1;
    return n;
}

std::string trim(const std::string &str)
{
    size_t first = str.find_first_not_of(" \t\r\n");
    size_t last = str.find_last_not_of(" \t\r\n");
    return (first == std::string::npos || last == std::string::npos) ? "" : str.substr(first, last - first + 1);
}

std::vector<std::string> split(const std::string &str, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);

    while (std::getline(tokenStream, token, delimiter))
	{
        tokens.push_back(token);
    }
    return tokens;
}

bool    Request::parseMethods(const std::string& method)
{
    if (method == "GET")
        _method = GET;
    else if (method == "POST")
        _method = POST;
    else if (method == "DELETE")
        _method = DELETE;
    else
        return false;
    return true;
}

bool    Request::parseRequestLine(const std::string& line)
{
    std::vector<std::string> requestLine = split(line, ' ');

    if (requestLine.size() != 3)
    {
        _status = 400;
        return false;
    }

    if (!parseMethods(requestLine[0]))
    {
        _status = 405;
        return false;
    }

    _url = requestLine[1];
    
    if (requestLine[2] != "HTTP/1.1")
    {
        _status = 505;
        return false;
    }
    this->_state = PARSING_HEADERS;

    return true;
}

bool    Request::parseHeaders(const std::string& line)
{
    if (line == "\r")
    {
        this->_state = PARSING_BODY;
        return false;
    }
    size_t colonPos = line.find(':');
    if (colonPos != std::string::npos)
    {
        std::string key = line.substr(0, colonPos);
        std::string value = line.substr(colonPos + 1);
        _headers[key] = value;
    }
    else
    {
        _status = 400;
        return false;
    }
    return true;
}

void Request::parseBody()
{
    std::map<std::string, std::string>::const_iterator it = _headers.find("Content-length");

    if (it == _headers.end())
    {
        _status = 411;
        _state = PARSING_COMPLETE;
        return ;
    }

    int contentLen = parseBodyLength(it->second);

    if (contentLen == -1)
    {
        _status = 400;
        _state = PARSING_COMPLETE;
        return ;
    }

    _body = _raw.str(); 

    if (_body.size() != static_cast<unsigned int>(contentLen))
    {
        _status = 400;
        _state = PARSING_COMPLETE;
        return ;
    }
    std::cout << _body << std::endl;
}

void Request::parse()
{
    std::string line;

    while (std::getline(_raw, line))
    {
        if (_state != PARSING_BODY && line[line.size() - 1] != '\r')
        {
            _status = 400;
            return ;
        }

        switch(_state)
        {
            case PARSING_REQUEST:
                if (!parseRequestLine(line))
                    return ;
                break;
            case PARSING_HEADERS:
                if (!parseHeaders(line))
                    return ;
                break;
            case PARSING_BODY:
                parseBody();
                break;
            case PARSING_COMPLETE:
                return ;
        }
    }


    // // Parse headers
    // while (std::getline(requestStream, line) && line != "\r")
	// {
    //     size_t pos = line.find(":");
    //     if (pos != std::string::npos)
	// 	{
    //         std::string headerName = trim(line.substr(0, pos));
    //         std::string headerValue = trim(line.substr(pos + 1));
    //         _headers[headerName] = headerValue;
    //     }
    // }

    // //IF THERE IS A BODY MUST USE CONTENT-LENGTH FIELD (orTransfer-Encoding: chunked)
    // //IF NO FIELD BUT BODY, BAD REQUEST

    // // Parse body (if any)
	// if (std::getline(requestStream, _body))
	// {
    //     _body = trim(_body);
    // }
}

std::map<std::string, std::string> Request::getHeaders() const
{
	return _headers;
}

//setters

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

Request::Request(const std::string& data): _status(200), _raw(data) {}