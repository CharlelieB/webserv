#include "Client.hpp"

int Client::getSd() const
{
	return _sd;
}

Client::Client(int sd): _sd(sd) {}