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
    int i = -1;
    headerCountLines = 0;
    std::vector<int> endIndexes;
    while (header[++i]) {
        if (!strncmp(&header[i], (const char *)"\r\n", 2)) {
            headerCountLines++;
            header[i] = '\0';
            endIndexes.push_back(i);
        }
    }
    char *line = header;
    headerCountLines--;
    i = 1;
    bool hasHost = false;
    for (std::vector<int>::const_iterator it = endIndexes.begin(); it != endIndexes.end(); ++it) {
        if (i != headerCountLines + 1) {
            switch (i++) {
                case 1 : isValidLine_First(line); break;
                default: isValidLine_KeyValue(line, hasHost); break;
            }
            line = &header[*it] + 2;
        }
    }
    if (!hasHost)
        throw NoHostInHeaderException();
}

void    HeaderValidation::isValidLine_First(std::string line) {
   if (getNumberOccurrences(line, " ") != 2 ||
            !ft_isalnum(line[0]) || !ft_isalnum(line[line.size() - 1]))
        throw HeaderValidationException(line);
    std::vector<std::string> splitted = ft_splitString(line, " ");
    if (splitted.size() != 3 || (splitted[0].compare("GET") && splitted[0].compare("POST")
        && splitted[0].compare("PUT") && splitted[0].compare("HEAD") ))
        throw HeaderValidationException(line);
}

void    HeaderValidation::isValidLine_KeyValue(std::string line, bool & hasHost) {
    if (!ft_isalnum(line[0]))
        throw HeaderValidationException(line);
    std::vector<std::string> spliited = ft_splitString(line, ": ");
    if (spliited.size() != 2)
        throw HeaderValidationException(line);
    if (!spliited[0].compare("Host"))
        hasHost = true;
}

size_t  HeaderValidation::getHeaderCountLines() const {
    return headerCountLines;
}