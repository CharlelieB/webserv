#include <iostream>
#include "Conf.hpp"
#include "Token.hpp"

int	main(int argc, char **argv)
{
	TokenRegistry tok;

	if (argc != 2)
	{
		std::cout << "Wrong arguments" << std::endl;
		return 1;
	}

	tok.lex(argv[1]);
	tok.parse();
}