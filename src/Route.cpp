#include "Route.hpp"

void Route::setLocation(const std::string& path)
{
	this->_location = path;
}

void Route::setMethods(const std::string& methods, bool enabled)
{
	this->_methods[methods] = enabled;
}

void Route::setRedirection(const std::string& url)
{
	this->_redirection = url;
}

void Route::setRoot(const std::string& root)
{
	this->_root = root;
}

void Route::setIndex(const std::string& index)
{
	this->_index = index;
}

void Route::setCgiPath(const std::string& path)
{
	this->_cgiPath = path;
}

void Route::setAutoIndex(bool enabled)
{
	this->_autoIndex = enabled;
}

std::string Route::getLocation() const
{
	return _location;
}

//Constructor
Route::Route(): _autoIndex(false) {}