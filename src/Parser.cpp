#include "Parser.hpp"
#include <iostream>
#include <sstream>

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

void Parser::parseString(std::string &str)
{
	advance();
	consume(WORD, "Expect an argument for the instruction");
	str = std::string(currentTokenValue());
	consume(SEMICOLON, "Expect at the end of instruction");
}

void Parser::parseInt(int &n)
{
	advance();
	consume(WORD, "Expect an argument for the instruction");
	std::istringstream ss(currentTokenValue());
	if (ss.fail())
	{
        throw std::runtime_error("Expect an integer");
    }
	ss >> n;
	consume(SEMICOLON, "Expect ; at the end of instruction");
}

void Parser::parseMultipleString(std::vector<std::string> &vec)
{
	advance();
	while (!isAtEnd() && !check(SEMICOLON))
	{
		consume(WORD, "Expect an argument for the instruction");
		vec.push_back(std::string(currentTokenValue()));
	}
	consume(SEMICOLON, "Expect ; at the end of instruction");
}

void Parser::handleListen(VirtualServer& server, const std::vector<std::string>& args)
{
	if (args.size() != 2)
		throw std::runtime_error("listen wrong number of arguments");
	//split the address with : delimiter
}

void Parser::handleServerName(VirtualServer& server, const std::vector<std::string>& args)
{
	if (args.size() < 2 || args.size() > 1000)
		throw std::runtime_error("server name wrong number of arguments");
	for (std::vector<std::string>::const_iterator it = args.begin() + 1; it != args.end(); ++it)
		server.serverNames.push_back(*it);
}
void Parser::handleErrorPage(VirtualServer& server, const std::vector<std::string>& args)
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
	server.errorPages.insert(std::make_pair(errorCode, args[2]));
}

void Parser::handleMaxBodySize(VirtualServer& server, const std::vector<std::string>& args)
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

	server.bodySize = size;
}

void Parser::setupHandlers()
{
    directiveHandlers["listen"] = &Parser::handleListen;
    directiveHandlers["server_name"] = &Parser::handleServerName;
    directiveHandlers["error_page"] = &Parser::handleErrorPage;
    directiveHandlers["client_max_body_size"] = &Parser::handleMaxBodySize;
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
			//create vectors of instructions
			std::vector<std::string> instructions;
			parseInstruction(instructions);

			if (instructions.empty())
				throw std::runtime_error("instruction");

            if (directiveHandlers.find(instructions[0]) != directiveHandlers.end())
			{
				//currentHandler = directiveHandlers[instructions[0]];
                //currentHandler(instructions);
				(this->*(directiveHandlers[instructions[0]]))(server, instructions);
			}
			// if (checkWord("listen"))
			// {
			// 	parseString(server.host);
			// }
			// else if (checkWord("root"))
			// {
			// 	parseString(server.rootDirectory);
			// }
			// else if (checkWord("server_name"))
			// {
			// 	parseMultipleString(server.serverNames);
			// }
			// else if (checkWord("error_page"))
			// {}
			// else if (checkWord("location")) {
			// 	server.locations.push_back(parseLocationBlock());
			// }
			// else
			// {
			// 	throw std::runtime_error("bad instruction");
			// }
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
	}
}

//Constructor

Parser::Parser(const std::vector<Token>& tokens, Configuration& config): tokens(tokens), config(config) {}