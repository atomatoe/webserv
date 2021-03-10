#include "HeaderValidation.hpp"

size_t getNumberOccurrences(std::string where, std::string find) {
    size_t count = 0;
    for(std::string::size_type i = 0; i < where.length(); ++i)
        if(where[i] == find[0])
            if(where.substr(i, find.length()) == find)
            {
                ++count;
                i += find.length() - 1;
            }
    return count;
}

void    HeaderValidation::valid(char *header) {
    headerCountLines = 0;
    char 	*tmp = strstr(header, "\r\n\r\n");
    size_t len = tmp - header;
    char *subStr = ft_substr(header, 0, len);
    std::vector<std::string> spliited = ft_splitString(subStr, "\r\n");
    bool hasHost = false;
    for (std::vector<std::string>::const_iterator it = spliited.begin(); it != spliited.end(); ++it) {
        if (it == spliited.begin())
            isValidLine_First(*it, subStr);
        else
            isValidLine_KeyValue(*it, subStr, hasHost);
    }
    free(subStr);
    if (!hasHost) {
        throw NoHostInHeaderException();
    }
}

void    HeaderValidation::isValidLine_First(std::string line, char *subStr) {
   if (getNumberOccurrences(line, " ") != 2 ||
            !ft_isalnum(line[0]) || !ft_isalnum(line[line.size() - 1]))
        throw HeaderValidationException(line, subStr);
    std::vector<std::string> splitted = ft_splitString(line, " ");
    if (splitted.size() != 3 || (splitted[0].compare("GET") && splitted[0].compare("POST")
        && splitted[0].compare("PUT") && splitted[0].compare("HEAD") ))
        throw HeaderValidationException(line, subStr);
}

void    HeaderValidation::isValidLine_KeyValue(std::string line, char *subStr, bool & hasHost) {
    if (!ft_isalnum(line[0]))
        throw HeaderValidationException(line, subStr);
    std::vector<std::string> spliited = ft_splitString(line, ": ");
    if (spliited.size() != 2)
        throw HeaderValidationException(line, subStr);
    if (!spliited[0].compare("Host"))
        hasHost = true;
}

size_t  HeaderValidation::getHeaderCountLines() const {
    return headerCountLines;
}