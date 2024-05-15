#include "Parser.hpp"
#include <iostream>
#include <sstream>

int Parser::parseNumber(const std::string& str)
{
	int nb;

	std::istringstream ss(str);
	if (ss.fail())
	{
		throw std::runtime_error("not an integer");
    }
	ss >> nb;
	return nb;
}

void Parser::throwParseError(const std::string& message)
{
	throw std::runtime_error("Parse error: " + message);
}

// bool isValidPort(int port)
// {
// 	return 1 <= port && port >= 65535;
// }

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
		throwParseError(errorMessage);
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
		throwParseError("instruction missing ;");
}

void Parser::parseListen(VirtualServer& server, const std::vector<std::string>& args)
{
	if (args.size() != 2)
		throwParseError(args[0] + ": wrong number of arguments");
	
	try
	{
		int port = parseNumber(args[1]);
		server.setPort(port);
	}
	catch(std::exception &e)
	{
		throwParseError(args[0] + ": port is not an integer");
	}
}

void Parser::parseHost(VirtualServer& server, const std::vector<std::string>& args)
{
	if (args.size() != 2)
		throwParseError(args[0] + ": wrong number of arguments");
	server.setHost(args[1]);
}


void Parser::parseServerName(VirtualServer& server, const std::vector<std::string>& args)
{
	if (args.size() < 2 || args.size() > 1000)
		throwParseError(args[0] + ": wrong number of arguments");
	for (std::vector<std::string>::const_iterator it = args.begin() + 1; it != args.end(); ++it)
		server.setServerNames(*it);
}
void Parser::parseErrorPage(VirtualServer& server, const std::vector<std::string>& args)
{
	if (args.size() != 3)
		throwParseError(args[0] + ": wrong number of arguments");
	
	try
	{
		int errorCode = parseNumber(args[1]);
		//Not checking url format yet (maybe we should)
		server.setErrorPage(errorCode, args[2]);
	}
	catch(std::exception &e)
	{
		throwParseError(args[0] + ": errorCode is not an integer");
	}
}

void Parser::parseMaxBodySize(VirtualServer& server, const std::vector<std::string>& args)
{
	if (args.size() != 2)
		throwParseError(args[0] + ": wrong number of arguments");
	
	int size;
	std::istringstream ss(args[1]);
	if (ss.fail())
	{
		throwParseError(args[0] + ": size is not an integer");
    }
	ss >> size;

	server.setBodySize(size);
}

void Parser::parseLimit(Route& route, const std::vector<std::string>& args)
{
	if (args.size() < 2 || args.size() > 4)
		throwParseError(args[0] + ": wrong number of arguments");
	
	for (std::vector<std::string>::const_iterator it = args.begin() + 1; it != args.end(); ++it)
	{
		if (*it != "GET" && *it != "POST" && *it != "DELETE")
			throwParseError(args[0] + ": wrong method");
		route.setMethods(*it, true);
	}
}

void Parser::parseRedirection(Route& route, const std::vector<std::string>& args)
{
	//for now handle only 301 redirect so conf does not include the redirection type
	if (args.size() != 2)
		throwParseError(args[0] + ": wrong number of arguments");
	route.setRedirection(args[1]);
}

void Parser::parseRoot(Route& route, const std::vector<std::string>& args)
{
	if (args.size() != 2)
		throwParseError(args[0] + ": wrong number of arguments");
	route.setRoot(args[1]);
}

void Parser::parseAutoIndex(Route& route, const std::vector<std::string>& args)
{
	if (args.size() != 2)
		throwParseError(args[0] + ": wrong number of arguments");
	if (args[1] == "on")
		route.setAutoIndex(true);
	else if (args[1] == "off")
		route.setAutoIndex(false);
	else
		throwParseError(args[0] + ": wrong arguments");
}

void Parser::parseIndex(Route& route, const std::vector<std::string>& args)
{
	if (args.size() != 2)
		throwParseError(args[0] + ": wrong number of arguments");
	route.setIndex(args[1]);
}

void Parser::parseCgiPath(Route& route, const std::vector<std::string>& args)
{
	if (args.size() != 2)
		throwParseError(args[0] + ": wrong number of arguments");
	route.setCgiPath(args[1]);
}

void Parser::parseLocation(VirtualServer& server)
{
	Route route;

	consume(WORD, "expect a string for the location instruction");
	if (!check(WORD))
	{
		throwParseError("expect an argument for location");
	}
	route.setLocation(tokens[current++].value);
	consume(O_BRACKET, "expect a bracket for the location instruction");

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
					throwParseError("incomplete instruction in location block");
				}
				(this->*(directiveHandlersLocation[instructions[0]]))(route, instructions);
			}
	    }
		else
		{
			throwParseError("location block");
        }
		advance();
	}
}

void Parser::setupHandlersInstruction()
{
    directiveHandlers["listen"] = &Parser::parseListen;
    directiveHandlers["host"] = &Parser::parseHost;
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
					throwParseError("incomplete instruction in server block");
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
			throwParseError("server block");
		}
		advance();
    }
	throwParseError("server block");
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
        	this->parseServerBlock();
	    }
		else
		{
			throwParseError("unexpected token: " + tokens[current].value);
        }
		advance();
	}
}

//Constructor

Parser::Parser(const std::vector<Token>& tokens, Configuration& config): tokens(tokens), config(config) {}