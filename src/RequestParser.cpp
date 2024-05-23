#include "RequestParser.hpp"

#include <iostream>
#include <sstream>

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

bool    RequestParser::parseMethods(const std::string& method)
{
    if (method == "GET")
        _currentRequest.setMethod(_currentRequest.GET);
    else if (method == "POST")
        _currentRequest.setMethod(_currentRequest.POST);
    else if (method == "DELETE")
        _currentRequest.setMethod(_currentRequest.DELETE);
    else
        return false;
    return true;
}

size_t RequestParser::parseRequestLine(Client& client)
{
        size_t parsedBytes = 0;
        size_t pos = client.getBuffer().find("\r\n");
        if (pos != std::string::npos)
		{
            // Parse the request line here
            std::string line = client.getBuffer().substr(0, pos);
            std::vector<std::string> requestLine = split(line, ' ');
            if (requestLine.size() != 3)
	        {
                throw std::runtime_error("request line parse error");
            }

            if (!parseMethods(requestLine[0]))
                throw std::runtime_error("wrong method");

            _currentRequest.setUrl(requestLine[1]);

            if (requestLine[2] != "HTTP/1.1")
                throw std::runtime_error("wrong http version");

            parsedBytes = pos + 2;
        }
        return parsedBytes;
}   

size_t    RequestParser::parseHeaders(Client &client)
{
    size_t parsedBytes = 0;
    size_t pos = client.getBuffer().find("\r\n\r\n");

    if (pos != std::string::npos)
    {
        std::string headers = client.getBuffer().substr(0, pos);

        std::istringstream stream(headers);
        std::string headerLine;
        
        while (std::getline(stream, headerLine) && headerLine != "\r")
        {
            size_t colonPos = headerLine.find(':');
            if (colonPos != std::string::npos)
            {
                std::string key = headerLine.substr(0, colonPos);
                std::string value = headerLine.substr(colonPos + 1);

                value.erase(0, value.find_first_not_of(" \t\r\n"));
                value.erase(value.find_last_not_of(" \t\r\n") + 1);
                _currentRequest.addHeader(key, value);
            }
            else
            {
               throw std::runtime_error("bad header");
            }
        }

        parsedBytes = pos + 4;  // Include \r\n\r\n in the length
    }
    return parsedBytes;
}

void	RequestParser::parse(Client& client)
{
    size_t parsedLen = 0;
    while (!client.getBuffer().empty())
	{
    //     switch (_state)
	// 	{
    //         case START:
                parsedLen = parseRequestLine(client);
                if (parsedLen > 0)
				{
                    client.eraseFromBuffer(0, parsedLen);
                    // _state = HEADERS;
					//handle bad request here (set _currentResponse to 400)
                }
    //             break;
    //         case HEADERS:
    //             parsedLen = parseHeaders(client);
    //             if (parsedLen > 0)
	// 			{
    //             	client.eraseFromBuffer(0, parsedLen);
    //                 _state = BODY;
    //             }
    //             break;
    //         case BODY:
    //             parsedLen = parseBody(client);
    //             if (parsedLen > 0)
	// 			{
    //             	client.eraseFromBuffer(0, parsedLen);
    //                 _state = COMPLETE;
    //             }
    //             break;
    //         case COMPLETE:
    //             client.addRequest(_currentRequest);
	// 			client.addResponse(_currentResponse);
	// 			// Reset for the potential next request
    //             _currentRequest = Request();
    //             _currentResponse = Response();
    //             _state = START;
    //             break;
    //     }
    }
}

// RequestParser::RequestParser() : _state(START) {}

/*
Make sure we handle the next request (if there is any) even if the previous request was a bad request
so we should probably skip the content until next request line?
in fact this is pipelining and i am not sure we should implement this
*/