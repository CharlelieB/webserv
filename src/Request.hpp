#pragma once

#include <string>
#include <map>

class Request
{
	public:
		enum eMethods {
			GET,
			POST,
			DELETE
		};
		void parse(const std::string& data);
		std::map<std::string, std::string> getHeaders() const;
		void    setMethod(enum eMethods method);
		void    setUrl(const std::string& url);
		// void    setHttpVersion(const std::string& version);
		void	addHeader(const std::string& key, const std::string& value);
	private:
		enum eMethods _method;
		std::string _url;
		// std::string _host;
		// std::string _httpVersion;
		std::map<std::string, std::string> _headers;
		std::string _body;
};