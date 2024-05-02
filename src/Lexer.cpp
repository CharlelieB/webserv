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
	//server{listen 80;}

	while (it != str.end())
	{
		//std::cout << *it << std::endl;
		token = findToken(*it);
		if (token.genericType == SEPARATOR)
		{
			//std::cout << "test " << advance;
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
   		std::cerr << "Failed to open file.";
    	return ; //maybe throw ?
	}

	while (getline(file, line))
	{
		this->tokenize(line);
		// std::cout << line << std::endl;
	}

	//this->tokenVec.push_back(makeToken("", END, END));
	file.close();

	for (std::vector<Token>::iterator it = this->tokenVec.begin(); it != this->tokenVec.end(); ++it)
	{
		std::cout << it->type << " " << it->value << std::endl;
	}
}

const std::vector<Token>& Lexer::getTokens() const { return tokenVec; }