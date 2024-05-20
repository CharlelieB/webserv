#include "Client.hpp"

int Client::getSd() const
{
	return _sd;
}

void	Client::addRequest(Request request)
{
	_requests.push(request);
}

void	Client::setBuffer(const char *buffer)
{
	_buffer = buffer;
}

const std::string&	Client::getBuffer() const
{
	return _buffer;
}

void	Client::eraseFromBuffer(size_t pos, size_t len) 
{
	_buffer.erase(pos, len);
}

Client::Client(int sd): _sd(sd) {}