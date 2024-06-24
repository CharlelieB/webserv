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

void handleGetRequest()
{
    //check if ressource exist
    //open the file
    //read in chunk and send at the same time
    //how to handle error_page?
    //serveFile()
}

bool Client::parseHeader()
{
	std::vector<unsigned char> raw(_buffer, _buffer + std::strlen(_buffer));
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
        std::cout << "read " << bytesRead << std::endl;

        if (bytesRead < 0)
        {
            std::cout << "bad recv" << std::endl;
			return false;
        }
        else if (bytesRead == 0)
        {
            // No more data (connection closed)
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
    std::cout << "test " << totalBytesRead << std::endl;
    //_buffer[totalBytesRead] = '\0';
    std::cout << _buffer << std::endl;
    return true;
}

void Client::postRessource()
{
    if (_request.getHeaders().find("content-length") == _request.getHeaders().end())
    {
        _status = 411;
        return;        
    }

    //open file
    std::ofstream outFile(_response.getPath().c_str(), std::ios::binary);

    if (!outFile.is_open())
    {
        std::cerr << "Failed to open the file!" << std::endl;
        _status = 500;
        return;
    }

    std::size_t bytesToWrite = _raw.size() - _cursor;

    //we write the body we got from the first recv
    const unsigned char *c_str = _raw.data();
    outFile.write(reinterpret_cast<const char*>(c_str + _cursor), bytesToWrite);

    size_t bodyLen = Utils::strToNb(_request.getHeaders().find("content-length")->second);
    
    if (bodyLen - bytesToWrite != 0)
    {
        //ssize_t totalRead = 0, bytesRead = 0;
        ssize_t bytesRead = 0;
        
        char* buffer = new char[bodyLen + 1];
        for (;;)
        {
            bytesRead = recv(_sd, _buffer + bytesRead, 4096, 0);

            if (bytesRead == -1)
                //check macro (two are fine other are error)
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

// void Client::setCGIEnv(std::vector<std::string>& env) const
// {
//     env.push_back("REQUEST_METHOD=" + _request.getMethod());
//     env.push_back("QUERY_STRING=" + queryString);
//     env.push_back("SCRIPT_NAME=" + scriptName);
//     env.push_back("SERVER_SOFTWARE=" + serverSoftware);
//     env.push_back("SERVER_NAME=" + serverName);
//     env.push_back("SERVER_PROTOCOL=" + serverProtocol);
//     env.push_back("SERVER_PORT=" + serverPort);
//     env.push_back("REMOTE_ADDR=" + remoteAddr);
//     env.push_back("REMOTE_HOST=" + remoteHost);
//     env.push_back("CONTENT_TYPE=" + contentType);
//     env.push_back("CONTENT_LENGTH=" + contentLength);
//     env.push_back("HTTP_USER_AGENT=" + httpUserAgent);
//     env.push_back("HTTP_REFERER=" + httpReferer);

// }

// void Client::execCGI() const
// {
//     setCGIEnv();
//     std::vector<std::string> env;

//     setCGIEnv(env);
//     char *scriptPath = _response.getPath().c_str();
//     static char *args[] = { "/usr/bin/php-cgi", scriptPath, NULL };

//     if (execve(args[0], args, NULL) == -1)
//     {
//         perror("execve failed");
//     }
// }

// bool Client::handleCGI() const
// {
//     pid_t pid;

//     int pid = fork();

//     switch (pid)
//     {
//         case -1:
//             perror("fork");
//             return false;
//         case 0:
//             execCGI();
//             //if we are here execve failed
//             return true;
//         default:
//             int status;
//             waitpid(-1, &status, 0);
//     }

// }

bool Client::sendResponse()
{
    if (!_mustSend)
        return true;

    if (!sendData(_response.getHeader()))
        return false;
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
            postRessource();
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
    if (!readHeader())
        return false;

    //std::vector<unsigned char> raw(_buffer, _buffer + std::strlen(_buffer));

    //_request.parse(raw); //in parsing we must check if we found the /r/n empty line (if no, request header too large)

    //_raw = raw;
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
//A request line cannot exceed the size of one buffer, or the 414
// (Request-URI Too Large) error is returned to the client. A request header
// field cannot exceed the size of one buffer as well, or the 400 (Bad Request)
// error is returned to the client. Buffers are allocated only on demand.

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