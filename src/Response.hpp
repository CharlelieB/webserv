#pragma once

#include <string>

class Response
{
	public:
		static std::unordered_map<int, std::string> statusMessage; 
		Response();
		void	build(const VirtualServer& server, const Request &request);
		void	generateHeader();
		void	getRessource();
		void	postRessource();
		void	buildDirectoryListing();
		bool	checkFile(const std::string& path);
		void	handleGet();
		void	checkLocationRules();
		void 	addIndex(const std::string &indexFile);
		void 	rootPath(const std::string &root, const std::string &baseUrl);
		void	reset();
		const	 std::string& getBody() const;
	private:
		int _statusCode;
		std::string _header;
		std::string _body;
		size_t _contentLength;
		std::string _ressourcePath;
		bool	_pathIsDir;
		Route *_route;
};