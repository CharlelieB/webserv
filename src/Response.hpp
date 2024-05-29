#pragma once

#include <string>

class Response
{
	public:
		std::unordered_map<int, std::string> statusMessage = 
		{
			{200, "OK"},
			{301, "Moved Permanently"},
			{400, "Bad Request"},
			{404, "Not Found"},
			{405, "Method Not Allowed"},
			{411, "Length Required"},
			{413, "Request Entity Too Large"}, //for client_max_body_size
			{500, "Internal Server Error"},
			{505, "HTTP Version Not Supported"}
		};
		Response();
		void	build(const VirtualServer& server, const Request &request);
		void	generateStartLine();
		void	getRessource();
		void	postRessource();
		void	buildDirectoryListing();
	private:
		int _statusCode;
		std::string _header;
		std::string _body;
		size_t _contentLength;
		std::string _rootedPath;
};