#include "Request.hpp"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include "utils.hpp"

std::vector<unsigned char> Request::getLine()
{
    std::vector<unsigned char> line;
    size_t endPos = std::find(_buffer.begin() + _pos, _buffer.end(), '\r') - _buffer.begin();
    if (endPos != _buffer.size() && endPos + 1 < _buffer.size() && _buffer[endPos + 1] == '\n')
	{
        line.insert(line.end(), _buffer.begin() + _pos, _buffer.begin() + endPos);
        _pos = endPos + 2; // Skip \r\n
    }
    return line;
}

int Request::parseBodyLength(const std::string& str)
{
    char* p;
    long n = strtol(str.c_str(), &p, 10);
    
    if (*p)
        return -1;
    return n;
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

void    Request::parseRequestLine()
{
	std::vector<unsigned char> requestLine = getLine();
    if (requestLine.empty())
	{
		_status = 400;
        return;
	}

    size_t methodEnd = std::find(requestLine.begin(), requestLine.end(), ' ') - requestLine.begin();
    
	if (methodEnd == requestLine.size())
    {	
		_status = 400;
		return;
	}

    size_t uriEnd = std::find(requestLine.begin() + methodEnd + 1, requestLine.end(), ' ') - requestLine.begin();
    if (uriEnd == requestLine.size())
	{
		_status = 400;
        return;
	}

    std::string method(requestLine.begin(), requestLine.begin() + methodEnd);
    std::string uri(requestLine.begin() + methodEnd + 1, requestLine.begin() + uriEnd);
    std::string httpVersion(requestLine.begin() + uriEnd + 1, requestLine.end());

    if (!parseMethods(method))
    {
        _status = 405;
        return;
    }

    _url = uri;
    std::cout << "debug ------------------------------ " << _url << std::endl;
    if (httpVersion != "HTTP/1.1")
    {
        _status = 505;
        return;
    }
}

bool    Request::parseHeaders()
{
	std::map<std::string, std::string> headers;
    while (_pos < _buffer.size())
	{
        std::vector<unsigned char> headerLine = getLine();
        if (headerLine.empty())
		{
            checkRequiredHeaderField();
        	return true;
        }
        size_t colonPos = std::find(headerLine.begin(), headerLine.end(), ':') - headerLine.begin();
        if (colonPos != headerLine.size())
		{
            std::string headerName(headerLine.begin(), headerLine.begin() + colonPos);
            std::string headerValue(headerLine.begin() + colonPos + 2, headerLine.end()); // skip ": "
            headerName = Utils::convertToLowercase(headerName);
            headerValue = Utils::convertToLowercase(headerValue);
			_headers[headerName] = headerValue;
        }
		else
		{
        	_status = 400;
        	return true;
		}
    }
    return false;
}

void Request::checkRequiredHeaderField()
{
    //if no host field, bad request
    std::map<std::string, std::string>::const_iterator it = _headers.find("host");

    if (it == _headers.end())
    {
        _status = 400;
        return;
    }

	if (_method == Methods::POST)
	{
		it = _headers.find("content-length");

		//Content length is required to post a body
		if (it == _headers.end())
		{
			_status = 411;
			return;
		}

		int contentLen = parseBodyLength(it->second);

		if (contentLen == -1)
		{
			_status = 400;
			return;
		}
	}
    // if (_body.size() != static_cast<unsigned int>(contentLen))
    // {
    //     _status = 400;
    //     _state = PARSING_COMPLETE;
    //     return ;
    // }
}

bool Request::parse(const std::vector<unsigned char>& buffer)
{
    _pos = 0;
    _buffer = buffer;

   parseRequestLine();
    if (_status == 200)
        if (!parseHeaders()) //if false, body is not found
            return false;
    return true;
}

void    Request::reset()
{
    _status = 200;
	_url.clear();
	_headers.clear();
    _buffer.clear();
    _pos = 0;
}

std::map<std::string, std::string> Request::getHeaders() const { return _headers; }

void	Request::addHeader(const std::string& key, const std::string& value) { _headers[key] = value; }

int Request::getStatus() const { return _status; }

Methods::eMethods Request::getMethod() const { return _method; }

std::string	Request::getUrl() const { return _url; }

size_t  Request::getPos() const { return _pos; }

Request::Request(): _status(200), _pos(0) {}