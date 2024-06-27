#pragma once

#include <queue>

#include "Request.hpp"
#include "Response.hpp"
#include "VirtualServer.hpp"
#include "const.hpp"

// #define DEBUG_CLIENT

class Client
{	
	public:
		Client(int sd, struct sockaddr_in* address, int addrlen);
		
		int	getSd() const;
		std::string getHost() const;
		int getPort() const;
		void	setBuffer(const char *buffer);


		bool 	parseHeader();
		bool	processRequest(const std::multimap<std::string, VirtualServer>& servers);
		bool 	sendResponse();
        bool    postRessource();
        bool    readHeader();
		bool 	sendData(const std::string& str);
        bool    serveFile();
		bool 	readBody();

		void	setCGIEnv(std::vector<std::string>& env) const;
		bool 	handleCGI() const;
		void 	execCGI() const;

		const VirtualServer*	findVirtualServer(const std::multimap<std::string, VirtualServer>& servers, const Request& req) const;
	private:
		Client();
		int	_sd;
		bool	_mustSend;
		std::string _host;
		int _port;
		std::vector<unsigned char> _raw;
		int _status;
		char _buffer[ConstVar::bufferSize + 1];
		unsigned char *_bodyBuffer;
        size_t _cursor;
		Request _request;
		Response _response;
};