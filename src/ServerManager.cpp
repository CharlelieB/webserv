#include "ServerManager.hpp"
#include <utility>

void	ServerManager::setAddressesToListen()
{
	for (std::vector<VirtualServer>::const_iterator it = _servers.begin(); it != _servers.end(); ++it)
	{
		if (std::find_if(_addresses.begin(), _addresses.end(), SearchPairFunctor(it->getHost(), it->getPort())) == _addresses.end())
		{
			_addresses.push_back(std::make_pair(it->getHost(), it->getPort()));
		}
	}
	// std::cout << "addr " << _addresses.size() << std::endl;
	// for (std::vector<std::pair<std::string, int> >::const_iterator it = _addresses.begin(); it != _addresses.end(); ++it)
	// 	std::cout << it->first << "  " << it->second << std::endl;
	
}

// void	ServerManager::run()
// {
// 	int sd;
//     struct sockaddr_in address;
//     int addrlen = sizeof(address);
//     char buffer[1024];

//     fd_set readfds, writefds, exceptfds;
//     std::map<int, std::string> message_queues;

//     // Define IP addresses and ports to listen on
//     std::vector<std::pair<std::string, int>> addresses = {
//         {"0.0.0.0", 12345},
//         {"127.0.0.1", 12346}
//     };

//     // Create and bind server sockets for each address
//     for (std::vector<std::pair<std::string, int>>::const_iterator addr = addresses.begin(); addr != addresses.end(); ++addr)
// 	{
//         int server_socket;
//         if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
// 		{
//             perror("socket failed");
//             exit(EXIT_FAILURE);
//         }

//         int opt = 1;
//         if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
// 		{
//             perror("setsockopt");
//             close(server_socket);
//             exit(EXIT_FAILURE);
//         }

//         address.sin_family = AF_INET;
//         address.sin_addr.s_addr = inet_addr(addr->first.c_str());
//         address.sin_port = htons(addr->second);

//         if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
// 		{
//             perror("bind failed");
//             close(server_socket);
//             exit(EXIT_FAILURE);
//         }

//         if (listen(server_socket, 3) < 0)
// 		{
//             perror("listen");
//             close(server_socket);
//             exit(EXIT_FAILURE);
//         }

//         _serverSockets.push_back(server_socket);
//         std::cout << "Listening on " << addr->first << ":" << addr->second << std::endl;
//     }

//     while (true)
// 	{
//         FD_ZERO(&readfds);
//         FD_ZERO(&writefds);
//         FD_ZERO(&exceptfds);

//         _maxFd = 0;

//         // Add server sockets to set
//         for (std::vector<int>::const_iterator serverSocket = _serverSockets.begin(); serverSocket != _serverSockets.end(); ++serverSocket)
// 		{
//             FD_SET(*serverSocket, &readfds);
//             if (*serverSocket > _maxFd)
// 			{
//                 _maxFd = *serverSocket;
//             }
//         }

//         // Add client sockets to set
//         for (std::vector<int>::const_iterator clientSocket = _clientSockets.begin(); clientSocket != _clientSockets.end(); ++clientSocket)
// 		{
//             sd = *clientSocket;
//             FD_SET(sd, &readfds);
//             FD_SET(sd, &writefds);
//             FD_SET(sd, &exceptfds);
//             if (sd > _maxFd)
// 			{
//                 _maxFd = sd;
//             }
//         }

//         int activity = select(_maxFd + 1, &readfds, &writefds, &exceptfds, nullptr);

//         if ((activity < 0) && (errno != EINTR))
// 		{
//             std::cerr << "select error" << std::endl;
//         }

//         // Handle new connections
//         for (std::vector<int>::const_iterator serverSocket = _serverSockets.begin(); serverSocket != _serverSockets.end(); ++serverSocket)
// 		{
//             if (FD_ISSET(*serverSocket, &readfds))
// 			{
//                 int newSocket = accept(*serverSocket, (struct sockaddr *)&address, (socklen_t*)&addrlen);
//                 if (newSocket < 0)
// 				{
//                     perror("accept");
//                     exit(EXIT_FAILURE);
//                 }
//                 std::cout << "New connection, socket fd is " << newSocket << ", ip is : " << inet_ntoa(address.sin_addr) << ", port: " << ntohs(address.sin_port) << std::endl;
//                 _clientSockets.push_back(newSocket);
//                 message_queues[newSocket] = "";
//             }
//         }

//         // Handle IO operations on client sockets
//         for (std::vector<int>::const_iterator clientSocket = _clientSockets.begin(); clientSocket != _clientSockets.end(); )
// 		{
//             sd = *clientSocket;
//             if (FD_ISSET(sd, &readfds))
// 			{
//                 int valread = read(sd, buffer, 1024);
//                 if (valread == 0)
// 				{
//                     getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
//                     std::cout << "Host disconnected, ip " << inet_ntoa(address.sin_addr) << ", port " << ntohs(address.sin_port) << std::endl;
//                     close(sd);
//                     clientSocket = _clientSockets.erase(clientSocket);
//                     message_queues.erase(sd);
//                 } 
// 				else
// 				{
//                     buffer[valread] = '\0';
//                     std::string message(buffer);
//                     std::cout << "Received message: " << message << std::endl;
//                     message_queues[sd] += message;
//                     ++clientSocket;
//                 }
//             }
// 			else if (FD_ISSET(sd, &writefds))
// 			{
//                 if (!message_queues[sd].empty())
// 				{
//                     const std::string& msg = message_queues[sd];
//                     int sent_bytes = send(sd, msg.c_str(), msg.size(), 0);
//                     if (sent_bytes > 0)
// 					{
//                         message_queues[sd].erase(0, sent_bytes);
//                     }
//                 }
//                 ++clientSocket;
//             }
// 			else if (FD_ISSET(sd, &exceptfds))
// 			{
//                 std::cerr << "Exception on socket " << sd << std::endl;
//                 close(sd);
//                 clientSocket = _clientSockets.erase(clientSocket);
//                 message_queues.erase(sd);
//             }
// 			else
// 			{
//                 ++clientSocket;
//             }
//         }
//     }

//     for (std::vector<int>::const_iterator serverSocket = _serverSockets.begin(); serverSocket != _serverSockets.end(); ++serverSocket)
// 	{
//         close((*serverSocket));
//     }
// }

ServerManager::ServerManager(std::vector<VirtualServer> servers): _servers(servers) {}