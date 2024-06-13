#include <string>
#include "utils.hpp"

namespace ErrorPages
{
	inline const std::string getDefaultErrorPage(int status, const std::string &message)
	{
		std::string sstatus = Utils::nbToStr(status);
		return "<html>\r\n<head><title>" + sstatus + " " + message + "</title></head>\r\n<body>\r\n<center><h1>" + sstatus + " " + message + "</h1></center>\r\n";
	}
}