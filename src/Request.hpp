#pragma once

#include <string>
#include <map>
#include <sstream>
#include <vector>
#include <cstdio>

namespace Methods
{
	enum eMethods { GET, POST, DELETE };
}

class Request
{
	public:
		Request();

		bool	parse(const std::vector<unsigned char>& buffer);
		void    parseRequestLine();
		bool	parseHeaders();
		void	checkRequiredHeaderField();
		bool    parseMethods(const std::string& method);
		ssize_t		parseBodyLength(const std::string& str);
		std::vector<unsigned char> getLine();
		std::map<std::string, std::string> getHeaders() const;
		ssize_t		getContentLen() const;

		void	addHeader(const std::string& key, const std::string& value);
		void    reset();
		
		int	getStatus() const;
		size_t getPos() const;
		Methods::eMethods	getMethod() const;
		std::string	getUrl() const;
		std::string	getHost() const;
	private:
		Methods::eMethods _method;
		int	_status;
		ssize_t _contentLen;
		std::string _url;
		std::map<std::string, std::string> _headers;
		std::vector<unsigned char> _buffer;
		size_t _pos;
};