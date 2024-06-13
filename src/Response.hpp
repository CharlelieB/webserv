#pragma once

#include <string>
#include "VirtualServer.hpp"
#include "Route.hpp"

class Response
{
	public:
		static const std::map<int, std::string> _statusMessage;
		Response();
		void	build(const VirtualServer& server, const Request &request);
		void	generateHeader();
		void	getRessource();
		void	postRessource();
		void	buildDirectoryListing();
		bool	ressourceExists(const std::string& path);
		void	handleGet();
		void	setupRoute(const Route& route);
		void	manageRouting(const Route* route, const Request &request);
		void 	addIndex(const std::string &indexFile);
		void 	rootPath(const std::string &root);
		void	reset();
		std::string getContent() const;
		void 	readCustomErrorPage(const std::string& path);
		static const std::map<int, std::string>& getStatusMessage();
		static const std::map<std::string, std::string>& getContentType();
		void	handleRequestByMethod(const Route* route, const Request &request);
		bool	isDirectory(const std::string& str);
		void	checkLocationRules(const Route& route);
		void normalizePath();
		void setContentType();
	private:
		int _statusCode;
		std::string _header;
		std::string _body;
		std::string _contentType;
		size_t _contentLength;
		std::string _ressourcePath;
		bool	_pathIsDir;
		//Route *_route;

		static std::map<int, std::string> createStatusMessageMap();
		static std::map<std::string, std::string> createContentTypeMap();
};