#pragma once

#include "Request.hpp"
#include "Client.hpp"

class RequestParser
{
	private:
		State		_state;
		Request		_currentRequest;
		Response	_currentResponse;
	public:
	    enum State { START, HEADERS, BODY, COMPLETE };
		RequestParser();
		void	parse(Client& client);
		size_t	parseRequestLine(Client& client);
		size_t    parseHeaders(Client& client);
};