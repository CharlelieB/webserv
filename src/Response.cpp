#include <vector>

#include "Response.hpp"
#include "Request.hpp"
#include "VirtualServer.hpp"

void	Response::build(const std::vector<VirtualServer>& servers, const Request &request)
{
	_statusCode = request.getStatus();

}

Response::Response() {}