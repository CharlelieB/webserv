#include "Conf.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include "Token.hpp"

bool isValidErrorCode(int code)
{
	return 400 <= code && code >= 599;
}

bool isValidPort(int port)
{
	return 1 <= port && port >= 65535;
}

// void tokenize(const std::string& line, std::vector<std::string> &tokens)
// {

// 	std::istringstream stream(line);
//     std::string token;

//     while (stream >> token) {
//         tokens.push_back(token);
//     }
// }



void Configuration::init(const char *filename)
{
	std::ifstream file;
	std::string line;
	//std::vector<std::string> tokens;

	TokenRegistry tokens;
	file.open(filename, std::ios::in);

	if (!file.is_open())
	{
   		std::cerr << "Failed to open file.";
    	return ; //maybe throw ?
	}

	while (getline(file, line))
	{
		tokens.tokenize(line);
		//std::cout << line << std::endl;
	}

	file.close();
	//parse the file here and set the virtual servers structures
}