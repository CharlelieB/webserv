#pragma once

#include <string>
#include <vector>

class TokenRegistry
{
	public:
		enum tokenType {
			O_BRACKET,
			C_BRACKET,
			SEMICOLON,
			SPACE,
			WORD,
			SEPARATOR,
			END
		};

		struct Token {
			std::string value;
			tokenType type;
			tokenType genericType;
		};

		static const Token tokens[];
		static const Token findToken(const char & c);

		void tokenize(const std::string& str);
		void lex(const char *filename);
		void parse() const;
		Token makeToken(const std::string &value, const enum tokenType type,
			enum tokenType generic);
	private:
		std::vector<Token> tokenVec;
};