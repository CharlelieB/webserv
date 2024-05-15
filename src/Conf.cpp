#include "Conf.hpp"
#include "Token.hpp"

Configuration::Configuration(const char *filename)
{
	this->_filename = filename;
}

void Configuration::parseConf()
{
	Lexer lex;
	lex.scan(this->_filename);

	Parser parser(lex.getTokens(), *this);
	parser.parseConfig();
}

//Setter
void Configuration::setServers(VirtualServer server) { _servers.push_back(server); }

const std::vector<VirtualServer>& Configuration::getServers() const { return _servers; };
