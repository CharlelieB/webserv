#pragma once

#include <string>

class Response
{
	public:
		static std::unordered_map<int, std::string> statusMessage; 
		Response();
		void	build(const VirtualServer& server, const Request &request);
		void	generateStartLine();
		void	getRessource();
		void	postRessource();
		void	buildDirectoryListing();
		bool	checkFile(const std::string& path);
		void	handleGet(const Route *route);
		void	checkLocationRules();
	private:
		int _statusCode;
		std::string _header;
		std::string _body;
		size_t _contentLength;
		std::string _rootedPath;
		bool	_pathIsDir;
};