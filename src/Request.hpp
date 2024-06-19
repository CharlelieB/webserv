#pragma once

#include <string>
#include <map>
#include <sstream>
#include <vector>

namespace Methods
{
	enum eMethods { GET, POST, DELETE };
}

class Request
{
	public:
		Request();

		void	parse(const std::vector<unsigned char>& buffer);
		bool    parseRequestLine();
		bool	parseHeaders();
		bool	checkRequiredHeaderField();
		bool    parseMethods(const std::string& method);
		int		parseBodyLength(const std::string& str);
		std::vector<unsigned char> getLine();
		std::map<std::string, std::string> getHeaders() const;

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
		std::string _url;
		std::map<std::string, std::string> _headers;
		std::vector<unsigned char> _buffer;
		size_t _pos;
};