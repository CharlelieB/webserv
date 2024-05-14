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
		void setServers(VirtualServer server);
		const std::vector<VirtualServer>& getServers() const;
		void parseConf();
	private:
		Configuration();
    	std::vector<VirtualServer> servers;
		const char *filename;
};