#pragma once

#include <queue>

#include "Request.hpp"
#include "Response.hpp"


class Client
{
	#define BUFFER_SIZE 8000
	
	private:
		Client();
		int	_sd;
		char	_buffer[BUFFER_SIZE];
		Request _request;
		Response _response;
	public:
		Client(int sd);
		int	getSd() const;
		void	setBuffer(const char *buffer);
		// const std::string&	getBuffer() const;
		bool	readRequest();
		void	buildResponse(const std::vector<VirtualServer>& _servers);
};