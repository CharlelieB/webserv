#pragma once

#include <queue>

#include "Request.hpp"
#include "Response.hpp"
#include "VirtualServer.hpp"

// #define DEBUG_CLIENT

class Client
{
	#define BUFFER_SIZE 8000
	
	public:
		Client(int sd, struct sockaddr_in* address, int addrlen);
		
		int	getSd() const;
		std::string getHost() const;
		int getPort() const;
		void	setBuffer(const char *buffer);

		bool	processRequest(const std::multimap<std::string, VirtualServer>& servers);
		bool	readRequest();
		bool	sendResponse();

		const VirtualServer*	findVirtualServer(const std::multimap<std::string, VirtualServer>& servers, const Request& req) const;
	private:
		Client();
		int	_sd;
		bool	_mustSend;
		char	_buffer[BUFFER_SIZE];
		std::string _host;
		int _port;
		Request _request;
		Response _response;
};