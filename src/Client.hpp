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
		const std::string&	getBuffer() const;
		void	eraseFromBuffer(size_t pos, size_t len);
		bool	readRequest();
};