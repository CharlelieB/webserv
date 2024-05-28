#include <unistd.h>
#include <sys/socket.h>
#include <iostream>

#include "Client.hpp"
#include "VirtualServer.hpp"

void	Client::buildResponse(const std::vector<VirtualServer>& servers)
{
	//find the right conf (virtual server) here
	_response.build(servers, _request);
}

void	Client::findVirtualServer(const std::vector<VirtualServer>& servers)
{

}

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

	this->_request.setRawData(s);
	this->_request.parse();
	return true;
}

int Client::getSd() const
{
	return _sd;
}

// const std::string&	Client::getBuffer() const
// {
// 	return _buffer;
// }

Client::Client(int sd): _sd(sd) {}