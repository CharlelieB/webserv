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
void Configuration::setServers(std::string host, VirtualServer server)
{
    _servers.insert(make_pair(host, server)); 
}

const std::multimap<std::string, VirtualServer>& Configuration::getServers() const { return _servers; };