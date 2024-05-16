#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <map>
#include <algorithm>

#include "VirtualServer.hpp"

class SearchPairFunctor
{
    public:
        SearchPairFunctor(const std::string& address, int port): _address(address), _port(port) {}
        bool operator()(std::pair<std::string, int> const & other)
		{
            return other.first == _address && other.second == _port;
        }
    private:
        std::string _address;
        int _port;
};

class ServerManager
{
	private:
		std::vector<int> _serverSockets;
    	std::vector<int> _clientSockets;
		int _maxFd;
		std::vector<std::pair<std::string, int> > _addresses;
		std::vector<VirtualServer> _servers;
	public:
		ServerManager(std::vector<VirtualServer> servers);
		void	run();
		void	setAddressesToListen();
};