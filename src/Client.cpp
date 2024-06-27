#include <string>
#include <map>
#include <vector>
#include <cstring>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "utils.hpp"
#include "const.hpp"
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

bool Client::serveFile()
{
    ssize_t bytesSent, totalSent = 0, bytesLeft, chunkSize;
    std::ifstream file(_response.getPath().c_str(), std::ios::binary);

    if (!file.is_open())
    {
        std::cerr << "Failed to open the file: " << _response.getPath() << std::endl;
        _status = 500;
        return true;
    }

    ssize_t fileSize = _response.getContentLen();

	std::stringstream ss;
    ss << file.rdbuf();

    std::string str = ss.str();
    const char* buffer = str.c_str(); 
    
    file.close();

    for (;;)
    {
        if (totalSent >= fileSize)
            break;

        bytesLeft = fileSize - totalSent;
        chunkSize = (bytesLeft < ConstVar::bufferSize) ? bytesLeft : ConstVar::bufferSize;

        bytesSent = send(_sd, buffer + totalSent, chunkSize, 0);
        if (bytesSent < 0)
        {
            //debug purpose
            if (errno == EPIPE) {
                std::cerr << "Socket closed by client (EPIPE)" << std::endl;
            } else {
                perror("send");
            }
            return false;
        }
        totalSent += bytesSent;
                            std::cout << "END-------------------" << totalSent << " " << fileSize << " bytes sent " << bytesSent << std::endl;

    }

    return true;
}

bool Client::parseHeader()
{
	std::vector<unsigned char> raw(_buffer, _buffer + std::strlen(_buffer));
    _raw = raw;
	return _request.parse(raw);
}

bool Client::readHeader()
{
    size_t bytesLeft, bytesRead;
    size_t toRead = 4096;
    size_t totalBytesRead = 0;

	for (;;)
    {
		if (totalBytesRead > ConstVar::bufferSize)
		{
			std::cout << "Header to large" << std::endl;
			return false;
		}
        bytesLeft = toRead - totalBytesRead;

        bytesRead = recv(_sd, _buffer, bytesLeft, 0);
std::cout << "Read header nb of bytes " << bytesRead << std::endl;

        if (bytesRead < 0)
        {
            std::cout << "bad recv" << std::endl;
			return false;
        }
        else if (bytesRead == 0)
        {
            if (totalBytesRead == 0)
            {
                std::cout << "Connection closed by client" << std::endl;
            }
            return false;
        }
        totalBytesRead += bytesRead;

    	_buffer[totalBytesRead] = '\0';
		if (parseHeader())
			break;
    }
    std::cout << "Read header - Total read " << totalBytesRead << std::endl;
    //_buffer[totalBytesRead] = '\0';
    //std::cout << _buffer << std::endl;
    return true;
}

bool Client::postRessource()
{
    //open file
    std::ofstream outFile(_response.getPath().c_str(), std::ios::binary);

    if (!outFile.is_open())
    {
        std::cerr << "Failed to open the file!" << std::endl;
        _status = 500;
        return true;
    }

    std::size_t bytesToWrite = _raw.size() - _cursor;

    //we write the body we got from the first recv
    const unsigned char *cStr = _raw.data();

    std::cout << "size : ----------------------" << _raw.size() << std::endl;

    std::cout << "file name : ----------------------" << _response.getPath().c_str() << std::endl;

    outFile.write(reinterpret_cast<const char*>(cStr + _cursor), bytesToWrite);

    std::cout << "content ;en " << _request.getContentLen() << std::endl;

    size_t bodyLen = _request.getContentLen();
    
    if (bodyLen - bytesToWrite != 0)
    {
        //ssize_t totalRead = 0, bytesRead = 0;
        ssize_t bytesRead = 0;
        
        char* buffer = new char[bodyLen + 1];
        for (;;)
        {
            bytesRead = recv(_sd, _buffer + bytesRead, 4096, 0);
            if (bytesRead < 0)
            {
                return false;
            }
            if (bytesRead == 0)
            {
                // if (totalRead == 0)
                //     //connection closed
                //close fd, remove client, communication is done
                break;
            }

            //totalRead += bytesRead;
        }
        outFile.write(buffer, bytesRead);
        delete[] buffer;
    }
    outFile.close();
    return true;
}

bool Client::sendData(const std::string& str)
{
    const char* buffer = str.c_str();

    ssize_t toSend = strlen(buffer);
    ssize_t totalSent = 0, bytesLeft, chunkSize, bytesSent;

    for (;;)
    {
        if (totalSent >= toSend)
            break;

        bytesLeft = toSend - totalSent;
        chunkSize = (bytesLeft < ConstVar::bufferSize) ? bytesLeft : ConstVar::bufferSize;

        bytesSent = send(_sd, buffer + totalSent, chunkSize, 0);
        if (bytesSent < 0)
        {
            return false;
        }
        totalSent += bytesSent;
    std::cout << "total send " << totalSent << std::endl;
    }
    return true;
}

void Client::execCGI() const
{
    std::string path = _response.getPath(); 
    const char *scriptPath = path.c_str();

    static const char *args[] = { "/usr/bin/php-cgi", scriptPath, NULL };

    if (execve(args[0], const_cast<char* const*>(args), _response.getCGIEnv().data()) == -1)
    {
        perror("execve failed");
    }
}

// void Client::setupPipe() const
// {
//     int pipefd[2];
    
//     if (pipe(pipefd) == -1)
//     {
//         perror("pipe");
//         exit(EXIT_FAILURE);
//     }
    
//     //write(pipefd[1], postData.c_str(), postData.size());
    
//     close(pipefd[1]);
    
//     dup2(pipefd[0], STDIN_FILENO);
    
//     close(pipefd[0]);
// }

// bool Client::handleCGI() const
// {
//     pid_t pid;

//     pid = fork();

//     if (pid == -1)
//     {
//         perror("fork");
//         return false;
//     }
//     else if (pid == 0)
//     {
//         setupPipe();
//         execCGI();
//         exit(EXIT_FAILURE);
//     }
//     else
//     {
//         int status;
//         waitpid(pid, &status, 0);
//     }

// }

bool Client::sendResponse()
{
    if (!_mustSend)
        return true;

    std::cout << "Test repsponse" << std::endl;
    if (!sendData(_response.getHeader()))
        return false;
    std::cout << _status << "debugyessttt" << std::endl;

    if (_status == 200)
    {
        // if (_response.getIsCGI())
        //     handleCGI();
        if (_request.getMethod() == Methods::GET)
        {
            if (!serveFile())
            {
                return false;   
            }
        }
        else if (_request.getMethod() == Methods::POST)
        {
            if (!postRessource())
            {
                return false;
            }
            std::string postRes = "{\"status\": \"success\",\"message\": \"Your request has been processed successfully.\"}";
            sendData(postRes);
        }
    }

    if (_status != 200)
    {
        sendData(_response.getBody());
    }
    //close(_sd);
    _mustSend = false;
	_response.reset();
    return true;
}

bool Client::processRequest(const std::multimap<std::string, VirtualServer>& servers)
{
    std::cout << "Test requestt" << std::endl;
    
    if (!readHeader())
    {
        return false;
    }
    _cursor = _request.getPos();

	const VirtualServer *server = findVirtualServer(servers, _request);

	if (!server)
	{
		std::cerr << "something really bad happened (virtual server not found)" << std::endl;
		return false;
	}

	_response.build(*server, _request);
	_request.reset();
	_mustSend = true;

    _status = _response.getStatus();

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