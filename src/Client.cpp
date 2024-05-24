#include "Client.hpp"
#include <unistd.h>
#include <sys/socket.h>
#include <iostream>

bool	Client::readRequest()
{
	int bytesRead = read(_sd, _buffer, 8000);

	if (bytesRead == 0)
	{
	    //getpeername(_sd, (struct sockaddr*)&_address, (socklen_t*)&_addrlen);
	    //std::cout << "Host disconnected, ip " << inet_ntoa(_address.sin_addr) << ", port " << ntohs(_address.sin_port) << std::endl;
		close(_sd);
	    return false;
	}
	_buffer[bytesRead] = '\0';
	std::cout << _buffer << std::endl;

std::string s(_buffer, bytesRead);

	this->_request = Request(s);
	this->_request.parse();
	return true;
}

int Client::getSd() const
{
	return _sd;
}

const std::string&	Client::getBuffer() const
{
	return _buffer;
}

// void	Client::eraseFromBuffer(size_t pos, size_t len) 
// {
// 	_buffer.erase(pos, len);
// }

Client::Client(int sd): _sd(sd) {}