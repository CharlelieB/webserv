#include <iostream>
#include "../Request.hpp"

int main()
{
	Request req;

	std::string str = "GET /index.html HTTP/1.1\r\n"
        "Host: www.example.com\r\n"
        "Connection: keep-alive\r\n"
        "User-Agent: Mozilla/5.0\r\n"
        "Accept: text/html\r\n"
        "\r\n"
        "This is the body.";

	req.setRawData(str);
	req.parse();
	std::map<std::string, std::string> head = req.getHeaders();
	std::map<std::string, std::string>::const_iterator it = head.find("Host");
	std::cout << it->second << std::endl;
}