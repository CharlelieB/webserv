#pragma once

#include <string>

class Response
{
	public:
		Response();
		void	build(const VirtualServer& server, const Request &request);
	private:
		int _statusCode;
		std::string _body;
		size_t contentLength;
};