#include <iostream>
#include <string>

std::string alias(const std::string &root, const std::string &url, const std::string &baseUrl)
{
    std::string baseUrlWithSlash = baseUrl;
    if (baseUrlWithSlash[baseUrlWithSlash.size() - 1] != '/')
        baseUrlWithSlash += "/";

    std::string rootWithSlash = root;
    if (rootWithSlash[rootWithSlash.size() - 1] != '/')
        rootWithSlash += "/";

    std::string relativeUrl;
    if (url.find(baseUrlWithSlash) == 0)
        relativeUrl = url.substr(baseUrlWithSlash.size());
    else
        relativeUrl = url.substr(baseUrl.size());

    std::string fullPath = rootWithSlash + relativeUrl;

    return fullPath;
}

std::string rootPath(const std::string &root, const std::string &url)
{
    std::string fullPath = root + url;

    return fullPath;
}

int main() {
    std::string root = "/tmp/www/";
    //std::string baseUrl = "/kapouet/pouic/";
    std::string url = "/kapouet/pouic/toto/pouet";

    std::string rootedPath = rootPath(root, url);
    std::cout << "Rooted path: " << rootedPath << std::endl;

    return 0;
}