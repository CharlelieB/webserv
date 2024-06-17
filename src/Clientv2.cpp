#include <string>
#include <map>
#include <vector>
#include <cstring>
#define BUFFER_SIZE 4096
#include "Response.hpp"

class Client
{
    public:
		Client(int sd, struct sockaddr_in* address, int addrlen);
        void    processRequest();
        void    postRessource();
        void    readHeader();

		const VirtualServer*	findVirtualServer(const std::multimap<std::string, VirtualServer>& servers, const Request& req) const;
    private:
		Client();
        int	_sd;
		bool	_mustSend;
        std::string _host;
		int _port;
        
        struct Request
        {
            std::string _url;
            std::string _method;
            std::map<std::string, std::string> _headers;
        };

        std::vector<unsigned char> _raw;
        char _buffer[BUFFER_SIZE + 1];
        size_t _cursor;
        Request _request;
        Response _response;
};

void Client::serveFile()
{
    ssize_t bytesSent;
    std::ifstream file(filePath, std::ios::binary);
    
    if (!file.is_open()) {
        std::cerr << "Failed to open the file: " << filePath << std::endl;
        return;
    }

    //std::vector<char> buffer(4096);
    char buffer[4096];
    while (!file.eof())
    {
        file.read(buffer, 4096);
        std::streamsize bytesRead = file.gcount();
        
        if (bytesRead > 0)
        {
            while (totalSent < bytesRead)
            {
                bytesSent = send(_sd, buffer + totalSent, bytesRead - totalSent, 0);
                if (bytesSent < 0)
                {
                    return false;
                }
                totalSent += bytesSent;
            }
        }
    }
    file.close();
}
void handleGetRequest()
{
    //check if ressource exist
    //open the file
    //read in chunk and send at the same time
    //how to handle error_page?
    //serveFile()
}

void Client::readHeader()
{
    size_t bytesLeft, bytesRead;
    size_t toRead = 4096;
    size_t totalBytesRead = 0;

    while (totalBytesRead < toRead)
    {
        bytesLeft = toRead - totalBytesRead;

        bytesRead = recv(_sd, _buffer + totalBytesRead, bytesLeft, 0);

        if (bytesRead < 0)
        {
            //check macro (two are fine other are error)
        }
        else if (bytesRead == 0)
        {
            // No more data (connection closed)
            if (totalBytesRead == 0)
            {
                //connection closed
            }
            //here must save the fact that we read everything
            noDataLeft = true;
            break;
        }

        totalBytesRead += bytesRead;
    }
    _buffer[totalBytesRead] = '\0';
}

void Client::postRessource()
{
    //here we want to make sure the request has content-length header
    if (noContentLen)
        return false;

    //open file
    std::ofstream outFile(filename, std::ios::binary);

    if (!outFile.is_open()) {
        std::cerr << "Failed to open the file!" << std::endl;
        return 1;
    }

    std::size_t bytesToWrite = _raw.size() - _cursor;

    //we write the body we got from the first recv
    outFile.write(_raw.data() + _cursor, bytesToWrite);

    ssize_t totalRead, bytesRead;
    char buffer[BUFFER_SIZE];
    for (;;)
    {
        bytesRead = recv(_sd, _buffer, 4096, 0);

        if (bytesRead == -1)
            //check macro (two are fine other are error)
        if (bytesRead == 0)
        {
            if (totalRead == 0)
                //connection closed
            //close fd, remove client, communication is done
            break;
        }
        outFile.write(buffer, bytesRead);

        totalRead += bytesRead;
    }
    outFile.close();
}

void Client::processRequest()
{
    readHeader();

    std::vector<unsigned char> raw(_buffer, _buffer + std::strlen(_buffer));

    parse(); //in parsing we must check if we found the /r/n empty line (if no, request header too large)

    _response.build(_request);
    if (_request._method == "GET")
        handleGetRequest();
}
//A request line cannot exceed the size of one buffer, or the 414
// (Request-URI Too Large) error is returned to the client. A request header
// field cannot exceed the size of one buffer as well, or the 400 (Bad Request)
// error is returned to the client. Buffers are allocated only on demand.

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