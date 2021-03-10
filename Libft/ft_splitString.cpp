//
// Created by Quaro Tamarillo on 2/23/21.
//

#include <vector>
#include "string"

std::vector<std::string> ft_splitString(std::string s, std::string delimiter) {
    std::vector<std::string> result;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        s.erase(0, pos + delimiter.length());
        result.push_back(token);
    }
    result.push_back(s);
    return (result);
}
