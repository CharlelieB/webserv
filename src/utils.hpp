#pragma once

#include <string>
#include <sstream>

namespace Utils
{
	template <typename T>
	inline std::string nbToStr(T n)
	{
		std::ostringstream str;
		str << n;
		return str.str();
	}

	inline std::string convertToLowercase(const std::string& str) 
	{ 
    	std::string lowerStr = str;
		for (std::string::size_type i = 0; i < str.length(); ++i)
        	lowerStr[i] = std::tolower(str[i]);
		return lowerStr;
	}
}