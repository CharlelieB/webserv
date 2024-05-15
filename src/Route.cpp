#include "Route.hpp"

void Route::setLocation(const std::string& path)
{
	this->location = path;
}

void Route::setMethods(const std::string& methods, bool enabled)
{
	this->methods[methods] = enabled;
}

void Route::setRedirection(const std::string& url)
{
	this->redirection = url;
}

void Route::setDefaultFile(const std::string& file)
{
	this->defaultFile = file;
}

void Route::setRoot(const std::string& root)
{
	this->root = root;
}

void Route::setIndex(const std::string& index)
{
	this->index = index;
}

void Route::setCgiPath(const std::string& path)
{
	this->cgiPath = path;
}

void Route::setAutoIndex(bool enabled)
{
	this->autoIndex = enabled;
}

//Constructor
Route::Route(): autoIndex(false) {}