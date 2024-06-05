#include <iostream>
#include <string>
#include <vector>

// Function to find the longest common prefix between two strings
std::string longestCommonPrefix(const std::string& str1, const std::string& str2) {
    int minLength = std::min(str1.length(), str2.length());
    for (int i = 0; i < minLength; ++i) {
        if (str1[i] != str2[i]) {
            return str1.substr(0, i);
        }
    }
    return str1.substr(0, minLength);
}

// Function to find the best match string with the longest common prefix
std::string findBestMatch(const std::vector<std::string>& potentialMatches, const std::string& input) {
    std::string bestMatch;
    std::string longestPrefix;
    
    for (std::vector<std::string>::const_iterator it = potentialMatches.begin(); it != potentialMatches.end(); ++it) {
        std::string currentPrefix = longestCommonPrefix(*it, input);
        if (currentPrefix.length() > longestPrefix.length()) {
            longestPrefix = currentPrefix;
            bestMatch = *it;
        }
    }
    
    return bestMatch;
}

int main() {
    std::vector<std::string> potentialMatches;
    //potentialMatches.push_back("/home/dir");
    potentialMatches.push_back("/home/");
    potentialMatches.push_back("/home");

    std::string input1 = "/home/dir/index";
    std::string input2 = "/home";

    std::string match1 = findBestMatch(potentialMatches, input1);
    std::string match2 = findBestMatch(potentialMatches, input2);

    std::cout << "Input: " << input1 << ", Best Match: " << match1 << std::endl;
    std::cout << "Input: " << input2 << ", Best Match: " << match2 << std::endl;

    return 0;
}