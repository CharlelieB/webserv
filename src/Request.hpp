#pragma once

#include <string>
#include <map>
#include <sstream>

namespace Methods
{
	enum eMethods { GET, POST, DELETE };
}

class Request
{
	public:
		Request();
		enum eState {
			PARSING_REQUEST,
			PARSING_HEADERS,
			PARSING_BODY,
			PARSING_COMPLETE
		};

		void	setRawData(const std::string& data);
		void	parse();
		bool    parseRequestLine(const std::string& line);
		bool	parseHeaders(const std::string& line);
		bool    parseMethods(const std::string& method);
		void	parseBody(const std::istringstream& raw);
		int		parseBodyLength(const std::string& str);

		std::map<std::string, std::string> getHeaders() const;
		void    setMethod(Methods::eMethods method);
		void    setUrl(const std::string& url);
		// void    setHttpVersion(const std::string& version);
		void	addHeader(const std::string& key, const std::string& value);

		int	getStatus() const;
		Methods::eMethods	getMethod() const;
		std::string	getUrl() const;
	private:
		enum eState	_state;
		Methods::eMethods _method;
		int	_status;
		std::string _rawData;
		std::string _url;
		// std::string _host;
		// std::string _httpVersion;
		std::map<std::string, std::string> _headers;
		std::string _body;
};