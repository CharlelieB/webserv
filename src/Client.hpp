#pragma once

#include <queue>

#include "Request.hpp"
#include "Response.hpp"

class Client
{
	private:
		Client();
		int	_sd;
		std::string _buffer;
		std::queue<Request> _requests;
		std::queue<Response> _responses;
	public:
		Client(int sd);
		int	getSd() const;
		void	addRequest(Request request);
		void	setBuffer(const char *buffer);
		const std::string&	getBuffer() const;
		void	eraseFromBuffer(size_t pos, size_t len);
};