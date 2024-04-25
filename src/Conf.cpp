#include "Conf.hpp"


bool isValidErrorCode(int code)
{
	return 400 <= code && code >= 599;
}

bool isValidPort(int port)
{
	return 1 <= port && port >= 65535;
}

void Configuration::init(const char *filename)
{
	//parse the file here and set the virtual servers structures
}