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

//Setter
void Configuration::setServers(VirtualServer server) { servers.push_back(server); }
