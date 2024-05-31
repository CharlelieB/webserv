#pragma once

#include <map>
#include <vector>
#include <string>

#include "VirtualServer.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"

class Configuration
{
	public:
		Configuration(const char *filename);
		void setServers(std::string host, VirtualServer server);
		const std::multimap<std::string, VirtualServer>& getServers() const;
		void parseConf();
	private:
		Configuration();
    	std::multimap<std::string, VirtualServer> _servers;
		const char* _filename;
};