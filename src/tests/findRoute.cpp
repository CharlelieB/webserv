#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

struct Route {
	std::string data;
};

std::string normalizePath(const std::string& path)
{
    std::string normalized;
    std::istringstream stream(path);
    std::string segment;
    std::vector<std::string> segments;

    // Diviser le chemin par '/'
    while (std::getline(stream, segment, '/'))
	{
        if (!segment.empty()) {
            segments.push_back(segment);
        }
    }

    // Joindre les segments avec un seul '/'
    for (std::vector<std::string>::const_iterator it = segments.begin(); it != segments.end(); ++it)
	{
        normalized += "/" + *it;
    }

    if (!path.empty() && path[path.size() - 1] == '/' && !normalized.empty() && normalized != "/")
    {
        normalized += "/";
    }
    return normalized.empty() ? "/" : normalized;
}

std::string findMostFaithfulPath(const std::map<std::string, Route>& routes, const std::string& requestPath) 
{
    std::string normalizedRequestPath = normalizePath(requestPath);
            //std::cout << "debug " << normalizedRequestPath << std::endl;

    std::string bestMatch;
    size_t bestMatchSegments = 0;

    // Calculer le nombre de segments dans le chemin de requête
    size_t requestPathSegments = std::count(normalizedRequestPath.begin(), normalizedRequestPath.end(), '/') + 1;

    // Itérer à travers la map pour trouver un chemin correspondant au chemin de requête normalisé
    for (std::map<std::string, Route>::const_iterator it = routes.begin(); it != routes.end(); ++it) 
	{
        std::string normalizedRoutePath = normalizePath(it->first);
        // Calculer le nombre de segments dans le chemin de la route
        size_t routePathSegments = std::count(normalizedRoutePath.begin(), normalizedRoutePath.end(), '/') + 1;

        // Ignorer les chemins plus longs que le chemin de requête
        if (routePathSegments > requestPathSegments)
            continue;

        // Vérifier combien de segments correspondent au début du chemin normalisé
        std::istringstream requestStream(normalizedRequestPath);
        std::istringstream routeStream(normalizedRoutePath);
        std::string requestSegment, routeSegment;
        size_t matchSegments = 0;

        while (std::getline(requestStream, requestSegment, '/') && std::getline(routeStream, routeSegment, '/'))
		{
            std::cout << "debug :" << requestSegment << " - " << routeSegment << std::endl;
            if (requestSegment == routeSegment) 
                ++matchSegments;
            else 
                break;
        }

        // Mettre à jour la meilleure correspondance si la correspondance actuelle est plus fidèle et n'a pas de segments supplémentaires
        if (matchSegments > bestMatchSegments && matchSegments == routePathSegments)
		{
            bestMatch = it->first;
            bestMatchSegments = matchSegments;
        }
    }

    if (!bestMatch.empty())
        return "Found: " + bestMatch;
	else
    {
        //if no match check if / root
        if (routes.find("/") != routes.end())
            return "Found: /";
        return "Not Found";
    }
}

int main()
{
    std::map<std::string, Route> routes;

    // routes["/"] = Route();
    // routes["/about"] = Route();
    // routes["/contact"] = Route();
    // // routes["/dossier/"] = Route();
    // routes["/dossier/dossier2g/"] = Route();
    // routes["/dossier//dossier2/aa"] = Route();
    // routes["/dossier/dossier2//filge"] = Route();
    // routes["/dossier/dossier2/file/aa"] = Route();
    // routes["pouic"] = Route();
    // routes["/website"] = Route();
    routes["/website/"] = Route();

    std::string requestPath;
    std::string result;

    // requestPath = "/dossier/dossier2/file";
    // result = findMostFaithfulPath(routes, requestPath);
    // std::cout << result << std::endl;

    // requestPath = "/dossier//dossier2/file";
    // result = findMostFaithfulPath(routes, requestPath);
    // std::cout << result << std::endl;

    // requestPath = "/dossier/dossier2//file";
    // result = findMostFaithfulPath(routes, requestPath);
    // std::cout << result << std::endl;

    // requestPath = "/dossier/dossier2/file/";
    // result = findMostFaithfulPath(routes, requestPath);
    // std::cout << result << std::endl;

    // requestPath = "/dossier/dossier2/file";
    // result = findMostFaithfulPath(routes, requestPath);
    // std::cout << result << std::endl;

    // requestPath = "/dossier/dossier2g/file/blabla/blabla";
    // result = findMostFaithfulPath(routes, requestPath);
    // std::cout << result << std::endl;

    // requestPath = "/kapouet/pouic/toto/pouet";
    // result = findMostFaithfulPath(routes, requestPath);
    // std::cout << result << std::endl;

    requestPath = "/website/html/app.html";
    result = findMostFaithfulPath(routes, requestPath);
    std::cout << result << std::endl;

    requestPath = "/website/";
    result = findMostFaithfulPath(routes, requestPath);
    std::cout << result << std::endl;

    return 0;
}