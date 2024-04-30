#pragma once

#include <string>
#include <vector>
#include "Token.hpp"
#include "Conf.hpp"

struct Node
{
	Token *token;
	Node *right;
	Node *left;
};

class Parser
{
	public:
	    Parser(const std::vector<Token>& tokens, Configuration& config);

		void parseConfig();
		void parseServerBlock();
		void parseInstruction(std::string& value, int args);


		void parseString(std::string &str);
		void parseInt(int &n);
		void parseMultipleString(std::vector<std::string> &str);

		bool check(tokenType type) const;
		bool checkNext(tokenType type) const;
		bool checkWord(const std::string &str) const;
		bool isAtEnd() const;
		void advance();

		Token consume(tokenType type, const std::string& errorMessage);
		Token consumeWord(const std::string &str, const std::string& errorMessage);
		const std::string& currentTokenValue() const;
		
		void setupHandlers();
	private:
		size_t current;
	    const std::vector<Token>& tokens;
		Node root;
		Configuration& config;

		typedef void (*HandlerFunction)(Configuration&, const std::string&);
		std::map<std::string, HandlerFunction> directiveHandlers;
};