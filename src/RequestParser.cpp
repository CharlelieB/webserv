#include "RequestParser.hpp"


// size_t parseRequestLine(Client& client)
// {
//         size_t parsedBytes = 0;
//         size_t pos = client.getBuffer().find("\r\n");
//         if (pos != std::string::npos)
// 		{
//             // Parse the request line here
//             parsedBytes = pos + 2;
//         }
//         return parsedBytes;
// }

void	RequestParser::parse(Client& client)
{
    size_t parsedLen = 0;
    while (!client.getBuffer().empty())
	{
        switch (_state)
		{
            case START:
                parsedLen = parseRequestLine(client);
                if (parsedLen > 0)
				{
                    client.eraseFromBuffer(0, parsedLen);
                    _state = HEADERS;
					//handle bad request here (set _currentResponse to 400)
                }
                break;
            case HEADERS:
                parsedLen = parseHeaders(client);
                if (parsedLen > 0)
				{
                	client.eraseFromBuffer(0, parsedLen);
                    _state = BODY;
                }
                break;
            case BODY:
                parsedLen = parseBody(client);
                if (parsedLen > 0)
				{
                	client.eraseFromBuffer(0, parsedLen);
                    _state = COMPLETE;
                }
                break;
            case COMPLETE:
                client.addRequest(_currentRequest);
				client.addResponse(_currentResponse);
				// Reset for the potential next request
                _currentRequest = Request();
                _currentResponse = Response();
                _state = START;
                break;
        }
    }
}

RequestParser::RequestParser() : _state(START) {}