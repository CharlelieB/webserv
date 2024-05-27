#pragma once

#include <string>

class Response
{
	public:
		Response();
		void	build(const std::vector<VirtualServer>& servers, const Request &request);
	private:
		int _statusCode;
		std::string _body;
		size_t contentLength;
};