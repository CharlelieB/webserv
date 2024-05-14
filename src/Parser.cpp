#include "Parser.hpp"
#include <iostream>
#include <sstream>

bool isValidErrorCode(int code)
{
	return 400 <= code && code >= 599;
}

bool isValidPort(int port)
{
	return 1 <= port && port >= 65535;
}

bool Parser::checkNext(tokenType type) const
{
	if ((current + 1) >= tokens.size()) return false;

	return tokens[current + 1].type == type;
}

bool Parser::check(tokenType type) const
{
    if (isAtEnd()) return false;
    return tokens[current].type == type;
}

const std::string& Parser::currentTokenValue() const
{
	return tokens[current].value;
}
	
bool Parser::isAtEnd() const {
	return current >= tokens.size();
}

bool Parser::checkWord(const std::string &str) const
{
	return tokens[current].value == str;
}

Token Parser::consumeWord(const std::string &str, const std::string& errorMessage)
{
	if (tokens[current].value != str)
		throw std::runtime_error(errorMessage);
	return tokens[current++];
}

Token Parser::consume(tokenType type, const std::string& errorMessage)
{
    if (!check(type))
		throw std::runtime_error(errorMessage);
    return tokens[current++];
}

void Parser::parseInstruction(std::vector<std::string>& args)
{
	while (!isAtEnd())
	{
		if (check(WORD))
			args.push_back(tokens[current++].value);
		else
			break;
	}
	if (!check(SEMICOLON))
		throw std::runtime_error("instruction parse error :");
}

void Parser::parseListen(VirtualServer& server, const std::vector<std::string>& args)
{
	if (args.size() != 2)
		throw std::runtime_error("listen wrong number of arguments");
	//split the address with : delimiter
	server.setHost(args[1]);
}

void Parser::parseServerName(VirtualServer& server, const std::vector<std::string>& args)
{
	if (args.size() < 2 || args.size() > 1000)
		throw std::runtime_error("server name wrong number of arguments");
	for (std::vector<std::string>::const_iterator it = args.begin() + 1; it != args.end(); ++it)
		server.setServerNames(*it);
}
void Parser::parseErrorPage(VirtualServer& server, const std::vector<std::string>& args)
{
	if (args.size() != 3)
		throw std::runtime_error("error_page wrong number of arguments");
	
	int errorCode;
	std::istringstream ss(args[1]);
	if (ss.fail())
	{
        throw std::runtime_error("error_page, error is not an integer");
    }
	ss >> errorCode;

	//Not checking url format yet (maybe we should)
	server.setErrorPage(errorCode, args[2]);
}

void Parser::parseMaxBodySize(VirtualServer& server, const std::vector<std::string>& args)
{
	if (args.size() != 2)
		throw std::runtime_error("max_body_size wrong number of arguments");
	
	int size;
	std::istringstream ss(args[1]);
	if (ss.fail())
	{
        throw std::runtime_error("max_body_size, size is not an integer");
    }
	ss >> size;

	server.setBodySize(size);
}

void Parser::parseLimit(Route& route, const std::vector<std::string>& args)
{
	if (2 < args.size() || args.size() > 4)
		throw std::runtime_error("limit_except wrong number of arguments");
	
	for (std::vector<std::string>::const_iterator it = args.begin() + 1; it != args.end(); ++it)
	{
		if (*it != "GET" && *it != "POST" && *it != "DELETE")
			throw std::runtime_error("limit_except wrong method");
		route.setMethods(*it, true);
	}
}

void Parser::parseRedirection(Route& route, const std::vector<std::string>& args)
{
	//for now handle only 301 redirect so conf does not include the redirection type
	if (args.size() != 2)
		throw std::runtime_error("redirection wrong number of arguments");
	route.setRedirection(args[1]);
}

void Parser::parseRoot(Route& route, const std::vector<std::string>& args)
{
	if (args.size() != 2)
		throw std::runtime_error("root wrong number of arguments");
	route.setRoot(args[1]);
}

void Parser::parseAutoIndex(Route& route, const std::vector<std::string>& args)
{
	if (args.size() != 2)
		throw std::runtime_error("autoindex wrong number of arguments");
	if (args[1] == "on")
		route.setAutoIndex(true);
	else if (args[1] == "off")
		route.setAutoIndex(false);
	else
		throw std::runtime_error("autoindex wrong argument");
}

void Parser::parseIndex(Route& route, const std::vector<std::string>& args)
{
	if (args.size() != 2)
		throw std::runtime_error("index wrong number of arguments");
	route.setIndex(args[1]);
}

void Parser::parseCgiPath(Route& route, const std::vector<std::string>& args)
{
	if (args.size() != 2)
		throw std::runtime_error("index wrong number of arguments");
	route.setCgiPath(args[1]);
}

void Parser::parseLocation(VirtualServer& server)
{
	Route route;

	consume(WORD, "Expect a string for the location instruction");
	if (!check(WORD))
		throw std::runtime_error("Expert an argument for location");
	route.setLocation(tokens[current++].value);
	consume(O_BRACKET, "Expect a bracket for the location instruction");

	while (!isAtEnd())
	{
		if (check(C_BRACKET))
		{
			return server.setRoutes(route);
		}
		if (check(WORD))
		{
 			if (directiveHandlersLocation.find(tokens[current].value) != directiveHandlersLocation.end())
			{
				//create vectors of instructions
				std::vector<std::string> instructions;
				parseInstruction(instructions);

				if (instructions.empty())
				{
					throw std::runtime_error("instruction");
				}
				(this->*(directiveHandlersLocation[instructions[0]]))(route, instructions);
			}
	    }
		else
		{
            std::cerr << "parse error near " << tokens[current].value << std::endl;
			return ;
        }
		advance();
	}
}

void Parser::setupHandlersInstruction()
{
    directiveHandlers["listen"] = &Parser::parseListen;
    directiveHandlers["server_name"] = &Parser::parseServerName;
    directiveHandlers["error_page"] = &Parser::parseErrorPage;
    directiveHandlers["client_max_body_size"] = &Parser::parseMaxBodySize;
}

void Parser::setupHandlersLocation()
{
    directiveHandlersLocation["limit_except"] = &Parser::parseLimit;
    directiveHandlersLocation["return"] = &Parser::parseRedirection;
    directiveHandlersLocation["root"] = &Parser::parseRoot;
    directiveHandlersLocation["autoindex"] = &Parser::parseAutoIndex;
    directiveHandlersLocation["index"] = &Parser::parseIndex;
    directiveHandlersLocation["cgi_path"] = &Parser::parseCgiPath;
}

void Parser::parseServerBlock()
{
	VirtualServer server;

	while (!isAtEnd())
	{
		if (check(C_BRACKET))
		{
			return this->config.setServers(server);
		}
		if (check(WORD))
		{
            if (directiveHandlers.find(tokens[current].value) != directiveHandlers.end())
			{
				//create vectors of instructions
				std::vector<std::string> instructions;
				parseInstruction(instructions);

				if (instructions.empty())
				{
					throw std::runtime_error("instruction");
				}
				(this->*(directiveHandlers[instructions[0]]))(server, instructions);
			}
			else if (tokens[current].value == "location")
			{
    			parseLocation(server);
			}
        }
		else
		{
			throw std::runtime_error("server block");
		}
		advance();
    }
	throw std::runtime_error("server block");
}

void Parser::advance()
{
	++current;
}

void Parser::parseConfig()
{
	this->current = 0;
	setupHandlersInstruction();
	setupHandlersLocation();

	while (!isAtEnd())
	{
        if (check(WORD) && checkWord("server") && checkNext(O_BRACKET))
		{
			current += 2;
			try
			{
        		this->parseServerBlock();
			}
			catch(std::runtime_error& e)
			{
				std::cout << e.what() << " parse error near " << tokens[current].value << std::endl;
				return ;
			}
	    }
		else
		{
            std::cerr << "parse error near " << tokens[current].value << std::endl;
			return ;
        }
		advance();
	}
}

/*

*/
//Constructor

Parser::Parser(const std::vector<Token>& tokens, Configuration& config): tokens(tokens), config(config) {}