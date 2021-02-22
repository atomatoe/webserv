#include "ParseConfig.hpp"

/* support */
const   std::string allowMethods[] = { "GET", "POST", "PUT", "HEAD"};
bool    ParseConfig::checkTabs(char *line, int countSpaces) {
    std::string strMustBe(countSpaces, ' ');
    std::string substr = std::string(line).substr(0, countSpaces);
    if (!substr.compare(strMustBe)) {
        if (ft_isspace(line[countSpaces]))
            exitError("Invalid number of spaces");
        return true;
    }
    if (countSpaces % 4 == 0) {
        int countTabs = countSpaces / 4;
        std::string strMustBe2(countTabs, '\t');
        std::string substr2 = std::string(line).substr(0, countTabs);
        if (!substr2.compare(strMustBe2)) {
            if (ft_isspace(line[countTabs]))
                exitError("Invalid number of tabs");
            return true;
        }
    }
    return false;
}
bool    ParseConfig::compareLines(char *line, std::string find) {
    char *trimmed = ft_strtrim(line, " \t");
    std::string trimmedStr = std::string(trimmed);
    free(trimmed);
    std::string result = trimmedStr.substr(0, find.size());
    if (!result.compare(find))
        return (true);
    return (false);
}


/* check main params */
bool    ParseConfig::checkServerName(char *line, WebServer &webServer) {
    if (compareLines(line, "server_name")) {
        if (!webServer.getServerName().empty())
            exitError("Duplicate param (server_name)");
        else {
            char *trimmedLine = ft_strtrim(line, " \t");
            char *trimmedValue = ft_strtrim(trimmedLine + 12, " \t");
            webServer.setServerName(trimmedValue);
            free(trimmedLine);
            free(trimmedValue);
            return (true);
        }
    }
    return (false);
}

bool    ParseConfig::checkIp(char *line, WebServer &webServer) {
    if (compareLines(line, "ip")) {
        if (!webServer.getIp().empty())
            exitError("Duplicate param (ip)");
        else {
            char *trimmedLine = ft_strtrim(line, " \t");
            char *trimmedValue = ft_strtrim(trimmedLine + 2, " \t");
            webServer.setIp(trimmedValue);
            free(trimmedLine);
            free(trimmedValue);
            return (true);
        }
    }
    return (false);
}

bool    ParseConfig::checkPort(char *line, WebServer &webServer) {
    if (compareLines(line, "port")) {
        if (webServer.getPort() != -1)
            exitError("Duplicate param (port)");
        else {
            char *trimmedLine = ft_strtrim(line, " \t");
            char *trimmedValue = ft_strtrim(trimmedLine + 4, " \t");
            if (!ft_str_is_num(trimmedValue))
                exitError("Incorrect symbol in port value");
            webServer.setPort(std::atoi(trimmedValue));
            free(trimmedLine);
            free(trimmedValue);
            return (true);
        }
    }
    return (false);
}

bool    ParseConfig::checkRoot(char *line, WebServer &webServer) {
    if (compareLines(line, "root")) {
        if (!webServer.getRootPath().empty())
            exitError("Duplicate param (root)");
        else {
            char *trimmedLine = ft_strtrim(line, " \t");
            char *trimmedValue = ft_strtrim(trimmedLine + 4, " \t");
            webServer.setRootPath(trimmedValue);
            free(trimmedLine);
            free(trimmedValue);
            return (true);
        }
    }
    return (false);
}

bool    ParseConfig::checkErrorPage(char *line, WebServer &webServer) {
    if (compareLines(line, "error_page")) {
        char *trimmed = ft_strtrim(line, " \t");
        char **splitted = ft_splitTim(trimmed, ' ');
        if (ft_strstrlen(splitted) != 3)
            exitError("Incorrect value of error_page");
        char *trimmedKey = ft_strtrim(splitted[1], " \t");
        char *trimmedValue = ft_strtrim(splitted[2], " \t");
        webServer.addErrorPage(trimmedKey, trimmedValue);
        free(trimmedValue);
        free(trimmedKey);
        ft_free_strstr(splitted);
        free(trimmed);
        return (true);
    }
    return (false);
}


/* check location params */
bool    ParseConfig::checkRootLoc(char *line, Location &location) {
    if (compareLines(line, "root")) {
        if (!checkTabs(line, 8))
            exitError("Invalid number of spaces or tabs");
        if (!location.getRoot().empty())
            exitError("Duplicate param (root)");
        else {
            char *trimmedLine = ft_strtrim(line, " \t");
            char *trimmedValue = ft_strtrim(trimmedLine + 4, " \t");
            location.setRoot(trimmedValue);
            free(trimmedLine);
            free(trimmedValue);
            return (true);
        }
    }
    return (false);
}

bool    ParseConfig::checkIndex(char *line, Location &location) {
    if (compareLines(line, "index")) {
        if (!checkTabs(line, 8))
            exitError("Invalid number of spaces or tabs");
        if (!location.getIndex().empty())
            exitError("Duplicate param (index)");
        else {
            char *trimmedLine = ft_strtrim(line, " \t");
            char *trimmedValue = ft_strtrim(trimmedLine + 5, " \t");
            location.setIndex(trimmedValue);
            free(trimmedLine);
            free(trimmedValue);
            return (true);
        }
    }
    return (false);
}

bool    ParseConfig::checkCgiPass(char *line, Location &location) {
    if (compareLines(line, "cgi_pass")) {
        if (!checkTabs(line, 8))
            exitError("Invalid number of spaces or tabs");
        char *trimmed = ft_strtrim(line, " \t");
        char **splitted = ft_splitTim(trimmed, ' ');
        if (ft_strstrlen(splitted) != 3)
            exitError("Incorrect value of cgi_pass");
        char *trimmedKey = ft_strtrim(splitted[1], " \t");
        char *trimmedValue = ft_strtrim(splitted[2], " \t");
        location.addCgiPath(trimmedKey, trimmedValue);
        free(trimmedValue);
        free(trimmedKey);
        ft_free_strstr(splitted);
        free(trimmed);
        return (true);
    }
    return (false);
}

bool    ParseConfig::checkAutoindex(char *line, Location &location) {
    if (compareLines(line, "autoindex")) {
        if (!checkTabs(line, 8))
            exitError("Invalid number of spaces or tabs");
        char *trimmedLine = ft_strtrim(line, " \t");
        char *trimmedValue = ft_strtrim(trimmedLine + 9, " \t");
        std::string trimmedValueStr(trimmedValue);
        if (!trimmedValueStr.compare("on"))
            location.setAutoIndex(true);
        else if (!trimmedValueStr.compare("off"))
            location.setAutoIndex(false);
        else
            exitError("Incorrect value of autoindex");
        free(trimmedLine);
        free(trimmedValue);
        return (true);
    }
    return (false);
}

bool    ParseConfig::checkLimitBody(char *line, Location &location) {
    if (compareLines(line, "limit_client_body_size")) {
        if (!checkTabs(line, 8))
            exitError("Invalid number of spaces or tabs");
        if (location.getLimitBody() != -1)
            exitError("Duplicate param (limit_client_body_size)");
        else {
            char *trimmedLine = ft_strtrim(line, " \t");
            char *trimmedValue = ft_strtrim(trimmedLine + ft_strlen("limit_client_body_size"), " \t");
            if (!ft_str_is_num(trimmedValue))
                exitError("Incorrect symbol in limit_client_body_size value");
            location.setLimitBody(std::atoi(trimmedValue));
            free(trimmedLine);
            free(trimmedValue);
            return (true);
        }
    }
    return (false);
}

bool    ParseConfig::checkAllowMethods(char *line, Location &location) {
    if (compareLines(line, "allow_methods")) {
        if (!checkTabs(line, 8))
            exitError("Invalid number of spaces or tabs");
        char *trimmedLine = ft_strtrim(line, " \t");
        char *trimmedValue = ft_strtrim(trimmedLine + ft_strlen("allow_methods"), " \t");
        std::string trimmedValueStr(trimmedValue);
        for (int i = 0; i < allowMethods->size(); i++) {
            if (!trimmedValueStr.compare(allowMethods[i])) {
                location.changeAllowMethod(allowMethods[i], true);
                free(trimmedLine);
                free(trimmedValue);
                return (true);
            }
        }
        exitError("Incorrect value in allow_methods");
    }
    return (false);
}


/* parse algorithm */
std::string     ParseConfig::parseLocation(char *line, WebServer &webServer) {
    bool (ParseConfig::*_checkFuncsLocation[6])(char *line, Location &location) = {
            &ParseConfig::checkRootLoc,
            &ParseConfig::checkIndex,
            &ParseConfig::checkCgiPass,
            &ParseConfig::checkAutoindex,
            &ParseConfig::checkLimitBody,
            &ParseConfig::checkAllowMethods
    };
    Location location;
    if (compareLines(line, "location")) {
        char *trimmedLine = ft_strtrim(line, " \t");
        char *trimmedValue = ft_strtrim(trimmedLine + 8, " \t");
        location.setUrl(trimmedValue);
        free(trimmedLine);
        free(trimmedValue);
        while (get_next_line(_fd, &line) >= 0) {
            _lineStr = line;
            char *trimmed = ft_strtrim(line, " \t");
            if (!trimmed) {
                free(trimmed);
                continue;
            }
            free(trimmed);
            if (_lineStr.empty()) {
                free(line);
                webServer.addLocation(location);
                return ("ok");
            }
            if (_lineStr[0] == '#') {
                continue;
            }
            else if (!_lineStr.compare("server"))
                break;
            else if (compareLines(line, "location")) {
                free(line);
                webServer.addLocation(location);
                return ("location");
            }
            else {
                bool findParam = false;
                for (int i = 0; i < 6; i++)
                    if ((findParam = (this->*_checkFuncsLocation[i])(line, location)))
                        break;
                if (!findParam)
                    exitError("Unknown parametr in \"" +  std::string(line) + "\"");
            }
            free(line);
        }
        free(line);
        webServer.addLocation(location);
        return ("ok");
    }
    return (std::string());
}

void            ParseConfig::parseServer(char *line) {
    bool (ParseConfig::*_checkFuncs[5])(char *line, WebServer &webServer) = {
            &ParseConfig::checkServerName,
            &ParseConfig::checkIp,
            &ParseConfig::checkPort,
            &ParseConfig::checkRoot,
            &ParseConfig::checkErrorPage
    };
    WebServer webServer;
    while (get_next_line(_fd, &line)) {
        _lineStr = line;
        char *trimmed = ft_strtrim(line, " \t");
        if (!trimmed) {
            free(trimmed);
            continue;
        }
        free(trimmed);
        if (_lineStr[0] == '#')
            continue;
        else if (!_lineStr.compare("server"))
            break;
        else if (checkTabs(line, 4)) {
                bool findParam = false;
                for (int i = 0; i < 5; i++) {
                    if ((findParam = (this->*_checkFuncs[i])(line, webServer)))
                        break;
                }
                std::string checkLoc;
                if (!findParam) {
                    checkLoc = parseLocation(line, webServer);
                    while (!checkLoc.compare("location")) {
                        findParam = true;
                        parseLocation(line, webServer);
                        checkLoc = parseLocation(line, webServer);
                    }
                    if (!checkLoc.compare("ok"))
                        findParam = true;
                }
                if (!_lineStr.compare("server"))
                    break;
                if (!findParam)
                    exitError("Unknown parametr in \"" +  std::string(line) + "\"");
        }
        else if (!_lineStr.empty())
            exitError("Invalid number of spaces or tabs");
        free(line);
    }
    free(line);
    if (webServer.getPort() == -1)
        exitError("Server cannot be without port");
    if (webServer.getIp().empty())
        exitError("Server cannot be without ip");
    _resultVector.push_back(webServer);
}


/* public */
ParseConfig::ParseConfig(std::string const fileName) : _fileName(fileName) { }

std::vector<WebServer>      ParseConfig::parse() {
    if ((_fd = open(_fileName.c_str(), O_RDONLY)) < 0)
        exitError("No such config (" + _fileName + ")");

    char *line = NULL;
    while (get_next_line(_fd, &line)) {
        _lineStr = line;
        if (_lineStr[0] == '#')
            continue;
        else if (_lineStr == "server") {
            while (_lineStr == "server")
                parseServer(line);
        }
        free(line);
    }
    free(line);
    close(_fd);
    return _resultVector;
}

void                        ParseConfig::printWebservers() {
    if (_resultVector.size() == 0)
        return;
    std::cout << "Count server: " << _resultVector.size() << std::endl << std::endl;

    std::vector<WebServer>::iterator it = _resultVector.begin();
    std::vector<WebServer>::iterator ite = _resultVector.end();
    while (it != ite) {
        std::cout << "server_name: " + (*it).getServerName() << std::endl;
        std::cout << "ip: " + (*it).getIp() << std::endl;
        std::cout << "port: "; std::cout << (*it).getPort() << std::endl;
        std::cout << "root: " + (*it).getRootPath() << std::endl;

        std::map<std::string, std::string> err_page = (*it).getErrorPage();
        std::map<std::string, std::string>::iterator it2 = err_page.begin();
        std::map<std::string, std::string>::iterator ite2 = err_page.end();
        while (it2 != ite2) {
            std::cout << "error_page: " + it2->first + " " + it2->second<< std::endl;
            it2++;
        }

        std::vector<Location> locations = (*it).getLocations();
        std::vector<Location>::iterator it3 = locations.begin();
        std::vector<Location>::iterator ite3 = locations.end();
        while (it3 != ite3) {
            std::cout << "location" << std::endl;
            std::cout << "\turl: " + (*it3).getUrl() << std::endl;
            std::cout << "\troot: " + (*it3).getRoot() << std::endl;
            std::cout << "\tindex: " + (*it3).getIndex() << std::endl;

            std::map<std::string, std::string> cgis = (*it3).getCgiPath();
            std::map<std::string, std::string>::iterator it4= cgis.begin();
            std::map<std::string, std::string>::iterator ite4 = cgis.end();
            while (it4 != ite4) {
                std::cout << "\tcgi_pass: " + it4->first + " " + it4->second<< std::endl;
                it4++;
            }

            std::cout << "\tautoindex: "; std::cout << (*it3).isAutoIndex() << std::endl;
            std::cout << "\tlimit_client_body_size: "; std::cout << (*it3).getLimitBody() << std::endl;

            std::map<std::string, bool> methods = (*it3).getAllowMethods();
            std::map<std::string, bool>::iterator it5= methods.begin();
            std::map<std::string, bool>::iterator ite5 = methods.end();
            while (it5 != ite5) {
                std::cout << "\tallow_method: " + it5->first + " ";
                std::cout << it5->second<< std::endl;
                it5++;
            }
            it3++;
        }
        std::cout << std::endl;
        it++;
    }
}
