#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>

#include "utils.hpp"
#include "Client.hpp"

const VirtualServer*	Client::findVirtualServer(const std::multimap<std::string, VirtualServer>& servers, const Request& req) const
{
	//because of parsing we are sure there is a Host field
	std::string host = req.getHeaders().find("Host")->second;

	//add default port to host header if no port
	if (host.find(":") == std::string::npos)
		host += ":80";

    std::pair<std::multimap<std::string, VirtualServer>::const_iterator, std::multimap<std::string, VirtualServer>::const_iterator> range;
    range = servers.equal_range(_host);

	if (range.first == range.second)
	{
		//search for a port matching with all addresses (*:port match)

    	std::string portStr = Utils::nbToStr(_port);
 
		range = servers.equal_range("0.0.0.0:" + portStr);
		if (range.first == range.second)
		{
			//normally this case should not happen, because we setup our sockets based on the conf file
			std::cerr << "no port match" << std::endl;
            return NULL;
		}
	}

	//filter by server_name (based on Host header field)
	std::multimap<std::string, VirtualServer>::const_iterator it = range.first;
	std::multimap<std::string, VirtualServer>::const_iterator begin = it;
	std::string port;

	for (; it != range.second; ++it)
	{
		port = Utils::nbToStr(it->second.getPort());
		for (std::vector<std::string>::const_iterator serverName = it->second.getServerNames().begin(); serverName != it->second.getServerNames().end(); ++serverName)
			if ((*serverName + ":" + port) == host)
				return &it->second;
	}

	//no match based on server_name, so return the first ip:port match
    return &begin->second;
}

bool	Client::readRequest()
{
	int bytesRead = read(_sd, _buffer, 8000);

	if (bytesRead == 0)
	{
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

bool	Client::processRequest(const std::multimap<std::string, VirtualServer>& servers)
{
	if (readRequest())
		return false;

	const VirtualServer *server = findVirtualServer(servers, _request);

	if (!server)
	{
		std::cerr << "something really bad happened (virtual server not found)" << std::endl;
		return false;
	}
	_response.build(*server, _request);
	_request.reset();
	_mustSend = true;
	return true;
}

bool	Client::sendRequest()
{
	if (!_mustSend)
		return;

    const char *buffer = _response.getBody().c_str();
    size_t length = _response.getBody().size();

    size_t totalSent = 0;
    const char *ptr = buffer;

    while (totalSent < length)
	{
        ssize_t bytesSent = send(_sd, ptr + totalSent, length - totalSent, 0);
        if (bytesSent < 0)
            return false;
        totalSent += bytesSent;
    }
	//after sending
	_mustSend = false;
	_response.reset();
}

int Client::getSd() const { return _sd; }

std::string Client::getHost() const { return _host; }

int Client::getPort() const { return _port; }

Client::Client(int sd, struct sockaddr_in* address, int addrlen): _sd(sd), _mustSend(false)
{
	//MUST CHECK FOR FAILURE
	getsockname(sd, (struct sockaddr*)address, (socklen_t*)&addrlen);
	_host = inet_ntoa(address->sin_addr);
    _port = ntohs(address->sin_port);

	#ifdef DEBUG_CLIENT
		getpeername(_sd, (struct sockaddr*)_address, (socklen_t*)&_addrlen);
		std::cout << "Client connected - " << inet_ntoa(_address->sin_addr) << ":" << ntohs(_address->sin_port);
		std::cout << " on server" << _host << ":" << port << std::endl;
	#endif
}