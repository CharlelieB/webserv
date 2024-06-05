#include <iostream>
#include <string>
#include <algorithm>

// Define a functor to replace the lambda function
struct IsDuplicate {
    char c;
    IsDuplicate(char c) : c(c) {}
    
    bool operator()(const char &c1, const char &c2) const {
        return c1 == c && c1 == c2;
    }
};

int main()
{
    std::string s = "/etc//var/dossier..//index//blabla/nlanla//"; 

    s.erase( std::unique(s.begin(), s.end(), IsDuplicate('/')), s.end()); 

    std::cout << s << '\n';             
}