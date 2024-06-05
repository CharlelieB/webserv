#include <iostream>
#include <string>
#include <vector>
#include <sstream>

std::string normalizePath(const std::string &path)
{
    std::vector<std::string> parts;
    std::stringstream ss(path);
    std::string part;
    
    while (std::getline(ss, part, '/'))
	{
        if (part == "" || part == ".") {
            continue;
        }
		else if (part == "..")
		{
            if (!parts.empty())
                parts.pop_back();
        }
		else
            parts.push_back(part);
    }

    std::string normalizedPath = "/";
    for (size_t i = 0; i < parts.size(); ++i) {
        normalizedPath += parts[i];
        if (i != parts.size() - 1) {
            normalizedPath += "/";
        }
    }

    if (!path.empty() && normalizedPath != "/" && path[path.size() - 1] == '/') {
        normalizedPath += "/";
    }
    
    return normalizedPath;
}

int main()
{
    std::string testPaths[] = {
        "/usr/local/../bin/",
        "./",
        "/",
        "/usr/../../..",
        "/home/user//docs/./notes.txt",
        "/var/www/html//../../etc/passwd",
        "//srv//www/../htdocs/index.html"
    };
    
    for (size_t i = 0; i < sizeof(testPaths) / sizeof(testPaths[0]); ++i) {
        std::cout << "Original: " << testPaths[i] << " -> Normalized: " << normalizePath(testPaths[i]) << std::endl;
    }
    
    return 0;
}