#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>

#include "utils.hpp"
#include "Client.hpp"

const VirtualServer*	Client::findVirtualServer(const std::multimap<std::string, VirtualServer>& servers, const Request& req) const
{	
	std::map<std::string, std::string> hosts = req.getHeaders();
	std::map<std::string, std::string>::iterator hostField = hosts.find("host");

	std::string host;
	//in case host field is missing, we provide an empty host to not match any server_name
	if (hostField == hosts.end())
		host = "";
	else
		host = hostField->second;

	if (!host.empty() && host.find(":") == std::string::npos) //add default port to host header if no port
		host += ":80";

    std::string portStr = Utils::nbToStr(_port);
    
	std::pair<std::multimap<std::string, VirtualServer>::const_iterator, std::multimap<std::string, VirtualServer>::const_iterator> range;

    range = servers.equal_range(_host + ":" + portStr);

	if (range.first == range.second)
	{
		//search for a port matching with all addresses (*:port match)

 
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
		const std::vector<std::string>& serverNames = it->second.getServerNames();
		for (std::vector<std::string>::const_iterator serverName = serverNames.begin(); serverName != serverNames.end(); ++serverName)
			if ((*serverName + ":" + port) == host)
				return &it->second;
	}

	//no match based on server_name, so return the first ip:port match
    return &begin->second;
}

bool	Client::readRequest()
{
	ssize_t bytesRead = recv(_sd, _buffer, ConstVar::bufferSize, 0);

	if (bytesRead <= 0)
	    return false;

	_buffer[bytesRead] = '\0';
	std::cout << _buffer << std::endl;

	std::string s(_buffer, bytesRead);

	this->_request.setRawData(s);
	this->_request.parse();

	return true;
}

bool	Client::processRequest(const std::multimap<std::string, VirtualServer>& servers)
{
	if (!readRequest())
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

bool	Client::sendResponse()
{
	if (!_mustSend)
		return true;

	std::string content = _response.getContent();
    const char *buffer = content.c_str();
    size_t length = content.size();

    size_t totalSent = 0;
    const char *ptr = buffer;

	//std::cout << buffer << std::endl;

    while (totalSent < length)
	{
	//std::cout << "SEND_-----------------------------------------------" << std::endl;
        ssize_t bytesSent = send(_sd, ptr + totalSent, length - totalSent, 0);
        if (bytesSent < 0)
		{
            return false;
		}
		totalSent += bytesSent;
    }
	//after sending
	_mustSend = false;
	_response.reset();
	return true;
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
		std::cout << " on server" << _host << ":" << _port << std::endl;
	#endif
}