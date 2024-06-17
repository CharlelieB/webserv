#include "Request.hpp"

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include "utils.hpp"

namespace Methods
{
	enum eMethods { GET, POST, DELETE };
}

class Request
{
	public:
		Request();

		void	setRawData(const std::string& data);
		void	parse();
		bool    parseRequestLine();
		bool	parseHeaders();
		bool	checkRequiredHeaderField();
		bool    parseMethods(const std::string& method);
		int		parseBodyLength(const std::string& str);

		std::map<std::string, std::string> getHeaders() const;

		void	addHeader(const std::string& key, const std::string& value);
		void    reset();
		
		int	getStatus() const;
		Methods::eMethods	getMethod() const;
		std::string	getUrl() const;
		std::string	getHost() const;
	private:
		Methods::eMethods _method;
		int	_status;
		std::string _url;
		std::map<std::string, std::string> _headers;
		const std::vector<unsigned char>& _buffer;
		size_t pos;
};

std::vector<unsigned char> getLine(const std::vector<unsigned char>& buffer, size_t& pos)
{
    std::vector<unsigned char> line;
    size_t endPos = std::find(buffer.begin() + pos, buffer.end(), '\r') - buffer.begin();
    if (endPos != buffer.size() && endPos + 1 < buffer.size() && buffer[endPos + 1] == '\n')
	{
        line.insert(line.end(), buffer.begin() + pos, buffer.begin() + endPos);
        pos = endPos + 2; // Skip \r\n
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

bool    Request::parseRequestLine(const std::string& line)
{
	std::vector<unsigned char> requestLine = getLine(_buffer, pos);

    if (requestLine.empty())
	{
		_status = 400;
        return false;
	}

    size_t methodEnd = std::find(requestLine.begin(), requestLine.end(), ' ') - requestLine.begin();
    
	if (methodEnd == requestLine.size())
    {	
		_status = 400;
		return false;
	}

    size_t uriEnd = std::find(requestLine.begin() + methodEnd + 1, requestLine.end(), ' ') - requestLine.begin();
    if (uriEnd == requestLine.size())
	{
		_status = 400;
        return false;
	}

    std::string method(requestLine.begin(), requestLine.begin() + methodEnd);
    std::string uri(requestLine.begin() + methodEnd + 1, requestLine.begin() + uriEnd);
    std::string httpVersion(requestLine.begin() + uriEnd + 1, requestLine.end());

    if (!parseMethods(method))
    {
        _status = 405;
        return false;
    }

    _url = uri;
    
    if (httpVersion != "HTTP/1.1")
    {
        _status = 505;
        return false;
    }
    return true;
}

bool    Request::parseHeaders(const std::string& line)
{
	std::map<std::string, std::string> headers;
    while (pos < _buffer.size())
	{
        std::vector<unsigned char> headerLine = getLine(_buffer, pos);
        if (headerLine.empty())
		{
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
        	return false;
		}
    }
    return true;
}

bool Request::checkRequiredHeaderField()
{
    //if no host field, bad request
    std::map<std::string, std::string>::const_iterator it = _headers.find("host");

    if (it == _headers.end())
    {
        _status = 400;
        return false;
    }

	if (_method == Methods::POST)
	{
		it = _headers.find("content-length");

		//Content length is required to post a body
		if (it == _headers.end())
		{
			_status = 411;
			return false;
		}

		int contentLen = parseBodyLength(it->second);

		if (contentLen == -1)
		{
			_status = 400;
			return false;
		}
	}
    // if (_body.size() != static_cast<unsigned int>(contentLen))
    // {
    //     _status = 400;
    //     _state = PARSING_COMPLETE;
    //     return ;
    // }
}

void Request::parse()
{
   if (!parseRequestLine())
	return;
   if (!parseHeaders() || !checkRequiredHeaderField())
	return;
}

void    Request::reset()
{
    _status = 200;
	_url.clear();
	_headers.clear();
}

std::map<std::string, std::string> Request::getHeaders() const { return _headers; }

void	Request::addHeader(const std::string& key, const std::string& value) { _headers[key] = value; }

void	Request::setRawData(const std::string& data) { _rawData = data; }

int Request::getStatus() const { return _status; }

Methods::eMethods Request::getMethod() const { return _method; }

std::string	Request::getUrl() const { return _url; }

Request::Request(): _state(PARSING_REQUEST), _status(200), _pos(0) {}