#pragma once

enum tokenType
{
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
