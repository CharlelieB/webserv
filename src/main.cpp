#include <iostream>
#include "Conf.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"

int	main(int argc, char **argv)
{
	Lexer lex;
	Configuration config;

	if (argc != 2)
	{
		std::cout << "Wrong arguments" << std::endl;
		return 1;
	}

	lex.scan(argv[1]);

	Parser parser(lex.getTokens(), config);
	parser.parseConfig();
}