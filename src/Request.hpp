#pragma once

#include <string>
#include <map>

class Request
{
	private:
		std::string _methods;
		std::string _url;
		// std::string _host;
		std::string _httpVersion;
		std::map<std::string, std::string> _headers;
		std::string _body;
	public:
		void parse(const std::string& data);
		std::map<std::string, std::string> getHeaders() const;
};