#pragma once

#include <string>
#include <vector>
#include "Token.hpp"

// #ifndef LEXER_DEBUG
// #define LEXER_DEBUG
// #endif

class Lexer
{
	public:
		static const Token tokens[];
		static const Token findToken(const char & c);

		void tokenize(const std::string& str);
		void scan(const char *filename);

		Token makeToken(const std::string &value, const enum tokenType type,
			enum tokenType generic);

		const std::vector<Token>& getTokens() const;
	private:
		std::vector<Token> tokenVec;
};