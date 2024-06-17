#include <string>
#include <map>
#include <vector>

class Client
{
    public:
        void    processRequest();
        void    postRessource();
    private:
        struct Request
        {
            std::string _url;
            std::string _method;
            std::map<std::string, std::string> _headers;
        };
    std::vector<unsigned char> _raw;
    size_t _cursor;
    Request _request;
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
    size_t bytesLeft;
    size_t toRead = 4096;
    size_t totalBytesRead = 0;

    while (totalBytesRead < toRead)
    {
        bytesLeft = toRead - totalBytesRead;

        bytesRead = recv(_sd, buffer + totalBytesRead, bytesLeft, 0);

        if (bytes_read < 0)
        {
            //check macro (two are fine other are error)
        }
        else if (bytes_read == 0)
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

        // Increment the total bytes read so far
        totalBytesRead += bytesRead;
    }
    buffer[totalBytesRead] = '\0';
}

void Client::postRessource()
{
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

    ssize_t totalRead;
    char buffer[4096 + 1];
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
        _buffer[bytesRead] = '\0';

        totalRead += bytesRead;
    }
    outFile.close();
}

void Client::processRequest()
{
    readHeader();

    std::vector<unsigned char> raw(_buffer, _buffer + strlen(_buffer));

    parse(); //in parsing we must check if we found the /r/n empty line (if no, request header too large)
    
    route();
    if (_request._method == "GET")
        handleGetRequest();
}
//A request line cannot exceed the size of one buffer, or the 414
// (Request-URI Too Large) error is returned to the client. A request header
// field cannot exceed the size of one buffer as well, or the 400 (Bad Request)
// error is returned to the client. Buffers are allocated only on demand.

