#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>


// int main(int argc, char **argv) 
// {
// 	if (argc != 3)
// 		return 1;

//     std::string loc = std::string(argv[1]);
//     std::string str2 = std::string(argv[2]);

//     if (compareFirstNCharacters(loc, str2)) {
//         std::cout << "match" << std::endl;
//     } else {
//         std::cout << "no match" << std::endl;
//     }

//     return 0;
// }

bool compareFirstNChar(const std::string& locationPath, const std::string& requestPath, size_t n)
{
	size_t reqLen = requestPath.size();

	if (reqLen < n)
		return false;

    return locationPath.compare(0, n, requestPath, 0, n) == 0;
}

struct Route {
	std::string data;
};


std::string findMostFaithfulPath(const std::map<std::string, Route>& routes, const std::string& requestPath)
{
	std::string match;
	std::string location;
	size_t locationLen = 0;
	size_t longest = 0;

	for (std::map<std::string, Route>::const_iterator it = routes.begin(); it != routes.end(); ++it)
	{
		location = it->first;
		locationLen = location.size();
		if (compareFirstNChar(location, requestPath, locationLen) && longest < locationLen)
		{
			match = location;
			longest = locationLen;
		}
	}
	return match.empty() ? "no match" : match; 
}

int main()
{
    std::map<std::string, Route> routes;

    routes["/"] = Route();
    routes["/about"] = Route();
    routes["/contact"] = Route();
    // routes["/dossier/"] = Route();
    routes["/dossier/dossier2g/"] = Route();
    routes["/dossier//dossier2/aa"] = Route();
    routes["/dossier/dossier2//filge"] = Route();
    routes["/dossier/dossier2/file/aa"] = Route();
    routes["pouic"] = Route();
    routes["/website"] = Route();

    std::string requestPath;
    std::string result;

    requestPath = "/dossier/dossier2/file";
    result = findMostFaithfulPath(routes, requestPath);
    std::cout << result << std::endl;

    requestPath = "/dossier//dossier2/file";
    result = findMostFaithfulPath(routes, requestPath);
    std::cout << result << std::endl;

    requestPath = "/dossier/dossier2//file";
    result = findMostFaithfulPath(routes, requestPath);
    std::cout << result << std::endl;

    requestPath = "/dossier/dossier2/file/";
    result = findMostFaithfulPath(routes, requestPath);
    std::cout << result << std::endl;

    requestPath = "/dossier/dossier2/file";
    result = findMostFaithfulPath(routes, requestPath);
    std::cout << result << std::endl;

    requestPath = "/dossier/dossier2g/file/blabla/blabla";
    result = findMostFaithfulPath(routes, requestPath);
    std::cout << result << std::endl;

    requestPath = "/kapouet/pouic/toto/pouet";
    result = findMostFaithfulPath(routes, requestPath);
    std::cout << result << std::endl;

    requestPath = "/website/html/app.html";
    result = findMostFaithfulPath(routes, requestPath);
    std::cout << result << std::endl;

    requestPath = "/websites";
    result = findMostFaithfulPath(routes, requestPath);
    std::cout << result << std::endl;

    return 0;
}