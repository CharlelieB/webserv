#include "Lexer.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

const Token Lexer::tokens[] = {
		{ "{", O_BRACKET, SEPARATOR },
		{ "}", C_BRACKET, SEPARATOR },
		{ ";", SEMICOLON, SEPARATOR },
		{ " ", SPACE, SEPARATOR },
		{ "", WORD, WORD }
		//{ "", END, END }
	};


const Token Lexer::findToken(const char & c)
{
	switch(c)
	{
		case ' ': case '\t':
			return tokens[SPACE];
			break;
		case '{':
			return tokens[O_BRACKET];
			break;
		case '}':
			return tokens[C_BRACKET];
			break;
		case ';':
			return tokens[SEMICOLON];
			break;
		default:
			return tokens[WORD];
	}
}

Token Lexer::makeToken(const std::string &value, const tokenType type, tokenType generic)
{
	Token token;
	
	token.value = value;
	token.type = type;
	token.genericType = generic;
	return token;
}

void Lexer::tokenize(const std::string& str)
{
	Token token;

	std::string::const_iterator it = str.begin();
	std::string::const_iterator start = str.begin();

	while (it != str.end())
	{
		token = findToken(*it);
		if (token.genericType == SEPARATOR)
		{
			if (it != start)
			{
				tokenVec.push_back(makeToken(std::string(start, it), WORD, WORD));
			}
			if (token.type != SPACE)
            {
				tokenVec.push_back(token);
            }
			++it;
			start = it;
		}
		else
			++it;
	}
	if (it != start)
	{
		tokenVec.push_back(makeToken(std::string(start, it), WORD, WORD));
	}
}

void Lexer::scan(const char *filename)
{
	std::ifstream file;
	std::string line;

	file.open(filename, std::ios::in);

	if (!file.is_open())
	{
    	throw(std::runtime_error("Failed to open conf file."));
	}

	while (getline(file, line))
	{
		this->tokenize(line);
	}

	file.close();

	#ifdef LEXER_DEBUG
	for (std::vector<Token>::iterator it = this->tokenVec.begin(); it != this->tokenVec.end(); ++it)
	{
		std::cout << it->type << " " << it->value << std::endl;
	}
	#endif
}

const std::vector<Token>& Lexer::getTokens() const { return tokenVec; }