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

Token Parser::consume(tokenType type, const std::string& errorMessage)
{
    if (!check(type)) throw std::runtime_error(errorMessage);
    	return tokens[current++];
}

// void Parser::parseInstruction(std::string& value, int args)
// {
// 	while (!isAtEnd() && args-- > 0)
// 	{

// 	}
// 	if (!check(SEMICOLON))
// 		throw std::runtime_error("instruction parse error :")
// }


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
	consume(SEMICOLON, "Expect at the end of instruction");
}

void Parser::parseMultipleString(std::vector<std::string> &vec)
{
	advance();
	while (!isAtEnd() && !check(SEMICOLON))
	{
		consume(WORD, "Expect an argument for the instruction");
		vec.push_back(std::string(currentTokenValue()));
	}
	consume(SEMICOLON, "Expect at the end of instruction");
}


void Parser::parseServerBlock()
{
	VirtualServer server;

	while (!isAtEnd())
	{
		if (check(O_BRACKET))
		{
			return this->config.setServers(server);
		}
		// if (check(WORD))
		// {
		// 	if (checkWord("listen"))
		// 	{
		// 		parseString(server.host);
		// 	}
		// 	if (checkWord("root"))
		// 	{
		// 		parseString(server.rootDirectory);
		// 	}
		// 	else if (checkWord("server_name"))
		// 	{
		// 		parseMultipleString(server.serverNames);
		// 	}
		// 	else if (checkWord("error_page"))
		// 	{}
		// 	else if (checkWord("location")) {
		// 		server.locations.push_back(parseLocationBlock());
		// 	}
		// 	else
		// 	{
		// 		throw std::runtime_error("bad instruction");
		// 	}
        // }
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
			advance();
        	this->parseServerBlock();
        }
		else
		{
            std::cerr << "parse error near " << tokens[current].value << std::endl; 
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