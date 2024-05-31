#include <iostream>
#include <string>

std::string addIndex(const std::string &path, const std::string &indexFile)
{
    if (path[path.size() - 1] == '/')
        return path + indexFile;
    return path;
}

int main() {
    std::string directoryPath1 = "/tmp/www/";
    std::string directoryPath2 = "/tmp/www/site/";
    std::string filePath = "/tmp/www/site/index.html";

	std::string index = "index.html";

    std::cout << "Directory path 1: " << addIndex(directoryPath1, index) << std::endl;
    std::cout << "Directory path 2: " << addIndex(directoryPath2, index) << std::endl;
    std::cout << "File path (should be unchanged): " << addIndex(filePath, index) << std::endl;

    return 0;
}