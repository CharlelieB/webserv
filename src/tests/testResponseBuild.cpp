#include "../Response.hpp"
#include "../VirtualServer.hpp"
#include "../Route.hpp"
#include "../Request.hpp"

int main()
{
	Response res;

	VirtualServer server;
	Route route;
	
	server.setHost("127.0.0.1");
	server.setPort(8080);
	server.setServerNames("exemple.com");
	server.setErrorPage(400, "/404.html");
	server.setBodySize(500000);
	server.setHost("127.0.0.1");
	
	route.setLocation("/");
	route.setMethods("GET", true);
	route.setIndex("index.html");
	route.setRoot("/var/www");

	Request req;

	req.setMethod(req.GET);
	req.setUrl("/");

	res.build(server, req);

}