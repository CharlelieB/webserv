#pragma once

#include <map>
#include <string>

class Route
{
	private:
		std::string _location;
		std::map<std::string, bool> _methods;
		std::string _redirection;
		std::string _root;
		std::string _index; //file to answer if the request path is a dir
		std::string _cgiPath;
		bool _autoIndex;
	public:
		Route();
		void setLocation(const std::string& path);
		void setMethods(const std::string& methods, bool enabled);
		void setRedirection(const std::string& url);
		void setRoot(const std::string& root);
		void setIndex(const std::string& index);
		void setCgiPath(const std::string& path);
		void setAutoIndex(bool enabled);

		std::string getLocation() const;
};