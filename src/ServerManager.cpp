#include <utility>
#include <cstdio>
#include <algorithm>
#include <cerrno>
#include "ServerManager.hpp"
#include "utils.hpp"

void	ServerManager::setAddressesToListen()
{
	for (std::multimap<std::string, VirtualServer>::const_iterator it = _servers.begin(); it != _servers.end(); ++it)
	{
		if (std::find_if(_addresses.begin(), _addresses.end(), SearchPairFunctor(it->second.getHost(), it->getPort())) == _addresses.end())
		{
			_addresses.push_back(std::make_pair(it->second.getHost(), it->second.getPort()));
		}
	}
	// std::cout << "addr " << _addresses.size() << std::endl;
	// for (std::vector<std::pair<std::string, int> >::const_iterator it = _addresses.begin(); it != _addresses.end(); ++it)
	// 	std::cout << it->first << "  " << it->second << std::endl;
	
}

void	ServerManager::handleNewConnections()
{
		for (std::vector<int>::const_iterator serverSocket = _serverSockets.begin(); serverSocket != _serverSockets.end(); ++serverSocket)
		{
            if (FD_ISSET(*serverSocket, &_readfds))
			{
                int newSocket = accept(*serverSocket, (struct sockaddr *)&_address, (socklen_t*)&_addrlen);
                if (newSocket < 0)
				{
                    perror("accept");
                    exit(EXIT_FAILURE);
                }
                std::cout << "New connection, socket fd is " << newSocket << ", ip is : " << inet_ntoa(_address.sin_addr) << ", port: " << ntohs(_address.sin_port) << std::endl;
				_clients.push_back(Client(newSocket, &_address, _addrlen));
            }
        }
}

void	ServerManager::addClientSocketsToSet()
{
	for (std::vector<Client>::iterator client = _clients.begin(); client != _clients.end(); ++client)
	{
        _sd = client->getSd();
        FD_SET(_sd, &_readfds);
        FD_SET(_sd, &_writefds);
        FD_SET(_sd, &_exceptfds);
        if (_sd > _maxFd)
		{
            _maxFd = _sd;
        }
    }
}

void	ServerManager::addServerSocketsToSet()
{
	for (std::vector<int>::const_iterator serverSocket = _serverSockets.begin(); serverSocket != _serverSockets.end(); ++serverSocket)
	{
	    FD_SET(*serverSocket, &_readfds);
	    if (*serverSocket > _maxFd)
		{
	        _maxFd = *serverSocket;
	    }
	}
}

void	ServerManager::run()
{
    _addrlen = sizeof(_address);

	setAddressesToListen();

    // Create and bind server sockets for each address
    for (std::vector<std::pair<std::string, int> >::const_iterator addr = _addresses.begin(); addr != _addresses.end(); ++addr)
	{
        int server_socket;
        if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		{
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        int opt = 1;
        if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		{
            perror("setsockopt");
            close(server_socket);
            exit(EXIT_FAILURE);
        }

        _address.sin_family = AF_INET;
        _address.sin_addr.s_addr = inet_addr(addr->first.c_str());
        _address.sin_port = htons(addr->second);

        if (bind(server_socket, (struct sockaddr *)&_address, sizeof(_address)) < 0)
		{
            perror("bind failed");
            close(server_socket);
            exit(EXIT_FAILURE);
        }

        if (listen(server_socket, 3) < 0)
		{
            perror("listen");
            close(server_socket);
            exit(EXIT_FAILURE);
        }

        _serverSockets.push_back(server_socket);
        std::cout << "Listening on " << addr->first << ":" << addr->second << std::endl;
    }

    while (true)
	{
        FD_ZERO(&_readfds);
        FD_ZERO(&_writefds);
        FD_ZERO(&_exceptfds);

        _maxFd = 0;

		addServerSocketsToSet();
		addClientSocketsToSet();

        int activity = select(_maxFd + 1, &_readfds, &_writefds, &_exceptfds, NULL);

        if ((activity < 0) && (errno != EINTR))
		{
            std::cerr << "select error" << std::endl;
        }

        // Handle new connections
		handleNewConnections();

        // Handle IO operations on client sockets
        for (std::vector<Client>::iterator client = _clients.begin(); client != _clients.end();)
		{
            _sd = client->getSd();
            if (FD_ISSET(_sd, &_readfds))
			{
                if (!client->processRequest(_servers))
                    client = this->removeClient(client);
                else
                    ++client;
            }
			else if (FD_ISSET(_sd, &_writefds))
			{
                if (!client->sendRequest())
                    client = this->removeClient(client);
                else
                    ++client;
            }
			else if (FD_ISSET(_sd, &_exceptfds))
			{
                std::cerr << "Exception on socket " << _sd << std::endl;
                client = this->removeClient(client);
            }
        }
    }

    for (std::vector<int>::const_iterator serverSocket = _serverSockets.begin(); serverSocket != _serverSockets.end(); ++serverSocket)
	{
        close((*serverSocket));
    }
}

std::vector<Client>::iterator ServerManager::removeClient(std::vector<Client>::iterator client)
{
    close(client->getSd());
    return _clients.erase(client);
}

ServerManager::ServerManager(const std::multimap<std::string, VirtualServer>&servers): _servers(servers) {}