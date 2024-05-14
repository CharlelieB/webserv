#include "Conf.hpp"
#include "Token.hpp"

Configuration::Configuration(const char *filename)
{
	this->filename = filename;
}

void Configuration::parseConf()
{
	Lexer lex;
	lex.scan(this->filename);

	Parser parser(lex.getTokens(), *this);
	parser.parseConfig();
}

//Setter
void Configuration::setServers(VirtualServer server) { servers.push_back(server); }

const std::vector<VirtualServer>& Configuration::getServers() const { return servers; };
