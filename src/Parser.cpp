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
		throw std::runtime_error("listen wrong arguments");
	
}

void Parser::handleServerName(VirtualServer& server, const std::vector<std::string>& args)
{
	if (args.size() < 2 || args.size() > 1000)
		throw std::runtime_error("server name wrong arguments");
	for (std::vector<std::string>::const_iterator it = args.begin() + 1; it != args.end(); ++it)
		server.serverNames.push_back(*it);
}
void Parser::handleErrorPage(VirtualServer& server, const std::vector<std::string>& args)
{

}

void Parser::handleMaxBodySize(VirtualServer& server, const std::vector<std::string>& args)
{
	
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
			//advance();
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


/*
class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}

    std::unique_ptr<ASTNode> parse() {
        auto root = std::make_unique<RootNode>();
        while (!isAtEnd()) {
            if (check(WORD) && peek().lexeme == "server" && checkNext(OPEN_BRACE)) {
                root->addChild(parseServerBlock());
            } else {
                advance();  // Skip tokens until a recognizable pattern starts
            }
        }
        return root;
    }

private:
    const std::vector<Token>& tokens;
    size_t current;

    bool check(TokenType type) {
        if (isAtEnd()) return false;
        return tokens[current].type == type;
    }

    bool checkNext(TokenType type) {
        if ((current + 1) >= tokens.size()) return false;
        return tokens[current + 1].type == type;
    }

    std::unique_ptr<ASTNode> parseServerBlock() {
        consume(WORD, "Expect 'server'");
        consume(OPEN_BRACE, "Expect '{'");
        auto block = std::make_unique<ServerBlockNode>();

        while (!check(CLOSE_BRACE)) {
            auto key = consume(WORD, "Expect property key").lexeme;
            auto value = consume(WORD, "Expect property value").lexeme;
            block->settings.emplace_back(key, value);
            consume(SEMICOLON, "Expect ';' after property");
        }

        consume(CLOSE_BRACE, "Expect '}' at end of server block");
        return block;
    }

    Token consume(TokenType type, const std::string& errorMessage) {
        if (!check(type)) throw std::runtime_error(errorMessage);
        return tokens[current++];
    }

    bool isAtEnd() const {
        return current >= tokens.size();
    }
};
*/


//Constructor

Parser::Parser(const std::vector<Token>& tokens, Configuration& config): tokens(tokens), config(config) {}