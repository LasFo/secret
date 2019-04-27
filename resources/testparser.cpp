#include <fstream>
#include <iostream>
#include <string>

int main(){
    std::ifstream ifs;

    ifs.open("text_data", std::ifstream::in);
    std::string s;
    getline(ifs, s);
    size_t off = 0, end = 0;
    std::string tmp;
    while(s.find("<<<", off) != std::string::npos)
    {
        end = s.find("<<<", off); 
        tmp = s.substr(off, end - off);
        off = end + 3;
        std::cout << tmp << std::endl;
    }

}
