#pragma once

#include <string>
#include <map>

class Request
{
	private:
		std::string _method;
		std::string _url;
		// std::string _host;
		std::string _httpVersion;
		std::map<std::string, std::string> _headers;
		std::string _body;
	public:
		void parse(const std::string& data);
		std::map<std::string, std::string> getHeaders() const;
		void    setMethod(const std::string& method);
		void    setUrl(const std::string& url);
		void    setHttpVersion(const std::string& version);
		void	addHeader(const std::string& key, const std::string& value);
};