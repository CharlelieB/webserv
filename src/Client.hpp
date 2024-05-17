#pragma once

#include "Request.hpp"
#include "Response.hpp"

class Client
{
	private:
		Client();
		int	_sd;
		Request request;
		Response response;
	public:
		Client(int sd);
		int getSd() const;
};