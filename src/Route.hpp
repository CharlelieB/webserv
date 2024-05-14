#pragma once

#include <map>
#include <string>

class Route
{
	private:
		std::string location;
		std::map<std::string, bool> methods;
		std::string redirections;
		bool directoryListing;
		std::string defaultFile;
		std::string root;
		std::string index;
		std::string cgiPath;
		bool autoIndex;
	public:
		void setLocation(const std::string& path);
		void setMethods(const std::string& methods, bool enabled);
		void setRedirection(const std::string& url);
		void setListing(bool enabled);
		void setDefaultFile(const std::string& file);
		void setRoot(const std::string& root);
		void setIndex(const std::string& index);
		void setCgiPath(const std::string& path);
		void setAutoIndex(bool enabled);
};