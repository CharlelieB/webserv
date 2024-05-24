#pragma once

#include <string>
#include <map>
#include <sstream>

class Request
{
	public:
		Request(const std::string& data);
		enum eState {
			PARSING_REQUEST,
			PARSING_HEADERS,
			PARSING_BODY,
			PARSING_COMPLETE
		};
		enum eMethods {
			GET,
			POST,
			DELETE
		};

		void	parse();
		bool    parseRequestLine(const std::string& line);
		bool	parseHeaders(const std::string& line);
		bool    parseMethods(const std::string& method);
		void	parseBody();
		int		parseBodyLength(const std::string& str);

		std::map<std::string, std::string> getHeaders() const;
		void    setMethod(enum eMethods method);
		void    setUrl(const std::string& url);
		// void    setHttpVersion(const std::string& version);
		void	addHeader(const std::string& key, const std::string& value);
	private:
		enum eState	_state;
		enum eMethods _method;
		int	_status;
		std::istringstream _raw;
		std::string _url;
		// std::string _host;
		// std::string _httpVersion;
		std::map<std::string, std::string> _headers;
		std::string _body;
};