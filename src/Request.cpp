#include "Request.hpp"

#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include "utils.hpp"

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
        _method = Methods::GET;
    else if (method == "POST")
        _method = Methods::POST;
    else if (method == "DELETE")
        _method = Methods::DELETE;
    else
        return false;
    return true;
}

bool    Request::parseRequestLine(const std::string& line)
{
    std::string requestLine = trim(line);

    std::vector<std::string> requestLineVec = split(requestLine, ' ');

    if (requestLineVec.size() != 3)
    {
        _status = 400;
        return false;
    }

    if (!parseMethods(requestLineVec[0]))
    {
        _status = 405;
        return false;
    }

    _url = requestLineVec[1];
    
    if (requestLineVec[2] != "HTTP/1.1")
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
        std::string key = Utils::convertToLowercase(line.substr(0, colonPos));
        std::string value = Utils::convertToLowercase(line.substr(colonPos + 1));
        _headers[key] = value;
    }
    else
    {
        _status = 400;
        return false;
    }
    return true;
}

void Request::parseBody(const std::istringstream& raw)
{
    //if no host field, bad request
    std::map<std::string, std::string>::const_iterator it = _headers.find("host");

    if (it == _headers.end())
    {
        _status = 400;
        _state = PARSING_COMPLETE;
        return ;
    }

    //no body
    if (raw.eof())
    {
        _state = PARSING_COMPLETE;
        return ;
    }

    it = _headers.find("content-length");
    //Content length is required to post a body
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

    _body = raw.str();

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
    std::istringstream raw(_rawData);
    std::string line;

    while (std::getline(raw, line))
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
                parseBody(raw);
                break;
            case PARSING_COMPLETE:
                return ;
        }
    }
}

void    Request::reset()
{
    _status = 200;
    _state = PARSING_REQUEST;
	_rawData.clear();
	_url.clear();
	_headers.clear();
	_body.clear();
}

std::map<std::string, std::string> Request::getHeaders() const { return _headers; }

void	Request::addHeader(const std::string& key, const std::string& value) { _headers[key] = value; }

void	Request::setRawData(const std::string& data) { _rawData = data; }

int Request::getStatus() const { return _status; }

Methods::eMethods Request::getMethod() const { return _method; }

std::string	Request::getUrl() const { return _url; }

Request::Request(): _state(PARSING_REQUEST), _status(200) {}