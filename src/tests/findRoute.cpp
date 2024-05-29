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

    return normalized.empty() ? "/" : normalized;
}

std::string findMostFaithfulPath(const std::map<std::string, Route>& route, const std::string& requestPath) 
{
    std::string normalizedRequestPath = normalizePath(requestPath);
    std::string bestMatch;
    size_t bestMatchSegments = 0;

    // Calculer le nombre de segments dans le chemin de requête
    size_t requestPathSegments = std::count(normalizedRequestPath.begin(), normalizedRequestPath.end(), '/') + 1;

    // Itérer à travers la map pour trouver un chemin correspondant au chemin de requête normalisé
    for (std::map<std::string, Route>::const_iterator it = route.begin(); it != route.end(); ++it) 
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

    // Retourner la meilleure correspondance si trouvée, sinon retourner "Not Found"
    if (!bestMatch.empty())
        return "Found: " + bestMatch;
	else 
        return "Not Found";
}

int main()
{
    std::map<std::string, Route> routes;

    routes["/home"] = Route();
    routes["/about"] = Route();
    routes["/contact"] = Route();
    routes["/dossier/"] = Route();
    routes["/dossier/dossier2g/"] = Route();
    routes["/dossier//dossier2/aa"] = Route();
    routes["/dossier/dossier2//filge"] = Route();
    routes["/dossier/dossier2/file/aa"] = Route();

    std::string requestPath = "/dossier/dossier2/file";
    std::string result = findMostFaithfulPath(routes, requestPath);
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

    return 0;
}