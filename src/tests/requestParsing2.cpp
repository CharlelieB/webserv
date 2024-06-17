#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <cstring>
#include <algorithm>

// Helper function to extract a line from the buffer
std::vector<unsigned char> getLine(const std::vector<unsigned char>& buffer, size_t& pos) {
    std::vector<unsigned char> line;
    size_t endPos = std::find(buffer.begin() + pos, buffer.end(), '\r') - buffer.begin();
    if (endPos != buffer.size() && endPos + 1 < buffer.size() && buffer[endPos + 1] == '\n') {
        line.insert(line.end(), buffer.begin() + pos, buffer.begin() + endPos);
        pos = endPos + 2; // Skip \r\n
    }
    return line;
}

void parseHttpRequest(const std::vector<unsigned char>& buffer)
{
    size_t pos = 0;

    std::vector<unsigned char> requestLine = getLine(buffer, pos);

    size_t methodEnd = std::find(requestLine.begin(), requestLine.end(), ' ') - requestLine.begin();
    size_t uriEnd = std::find(requestLine.begin() + methodEnd + 1, requestLine.end(), ' ') - requestLine.begin();

    std::vector<unsigned char> method(requestLine.begin(), requestLine.begin() + methodEnd);
    std::vector<unsigned char> uri(requestLine.begin() + methodEnd + 1, requestLine.begin() + uriEnd);
    std::vector<unsigned char> httpVersion(requestLine.begin() + uriEnd + 1, requestLine.end());

    std::cout << "Method: ";
    std::cout.write(reinterpret_cast<char*>(method.data()), method.size());
    std::cout << std::endl;

    std::cout << "URI: ";
    std::cout.write(reinterpret_cast<char*>(uri.data()), uri.size());
    std::cout << std::endl;

    std::cout << "HTTP Version: ";
    std::cout.write(reinterpret_cast<char*>(httpVersion.data()), httpVersion.size());
    std::cout << std::endl;

    // Parse headers
    std::map<std::string, std::string> headers;
    while (pos < buffer.size()) {
        std::vector<unsigned char> headerLine = getLine(buffer, pos);
        if (headerLine.empty()) {
            break; // End of headers
        }
        size_t colonPos = std::find(headerLine.begin(), headerLine.end(), ':') - headerLine.begin();
        if (colonPos != headerLine.size()) {
            std::string headerName(headerLine.begin(), headerLine.begin() + colonPos);
            std::string headerValue(headerLine.begin() + colonPos + 2, headerLine.end()); // skip ": "
            headers[headerName] = headerValue;
        }
    }

    // Print headers
    for (std::map<std::string, std::string>::const_iterator  it = headers.begin(); it != headers.end(); ++it) {
        std::cout << "Header: " << it->first << " = " << it->second << std::endl;
    }

    // Parse body if Content-Length header exists
    std::map<std::string, std::string>::const_iterator contentLengthIt = headers.find("Content-Length");
    if (contentLengthIt != headers.end()) {
        int contentLength = std::atoi(contentLengthIt->second.c_str());
        if (pos + contentLength <= buffer.size()) {
            std::vector<unsigned char> body(buffer.begin() + pos, buffer.begin() + pos + contentLength);
            std::cout << "Body: ";
            std::cout.write(reinterpret_cast<const char*>(body.data()), body.size());
            std::cout << std::endl;
        }
    }
}

int main()
{
    const char *request = "GET / HTTP/1.1\r\nHost: exampl.com\r\nContent-Length: 11\r\n\r\nHello world";

    std::vector<unsigned char> buffer(request, request + strlen(request));

    parseHttpRequest(buffer);

    return 0;
}