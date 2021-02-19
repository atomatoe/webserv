#ifndef PARSECONFIG_HPP
#define PARSECONFIG_HPP

//#include "Location.hpp"
#include "gnl/get_next_line.hpp"
#include "../WebServ.hpp"
#include "../includes/includes.hpp"
#include "../libft/libft.h"

class ParseConfig {
private:
    std::string             _fileName;
    int                     _fd;
    std::vector<WebServer>  _resultVector;
    std::string             _lineStr;

    /* support */
    bool            checkTabs(char *line, int countTabs);
    bool            compareLines(char *line, std::string find);

    /* check main params */
    bool            checkServerName(char *line, WebServer &webServer);
    bool            checkIp(char *line, WebServer &webServer);
    bool            checkPort(char *line, WebServer &webServer);
    bool            checkRoot(char *line, WebServer &webServer);
    bool            checkErrorPage(char *line, WebServer &webServer);
    bool (ParseConfig::*_checkFuncs[5])(char *line, WebServer &webServer) = {
            &ParseConfig::checkServerName,
            &ParseConfig::checkIp,
            &ParseConfig::checkPort,
            &ParseConfig::checkRoot,
            &ParseConfig::checkErrorPage
    };

    /* check location params */
    bool            checkRootLoc(char *line, Location &location);
    bool            checkIndex(char *line, Location &location);
    bool            checkCgiPass(char *line, Location &location);
    bool            checkAutoindex(char *line, Location &location);
    bool            checkLimitBody(char *line, Location &location);
    bool            checkAllowMethods(char *line, Location &location);

    bool (ParseConfig::*_checkFuncsLocation[6])(char *line, Location &location) = {
            &ParseConfig::checkRootLoc,
            &ParseConfig::checkIndex,
            &ParseConfig::checkCgiPass,
            &ParseConfig::checkAutoindex,
            &ParseConfig::checkLimitBody,
            &ParseConfig::checkAllowMethods
    };

    /* parse algorithm */
    void            parseServer(char *line);
    std::string     parseLocation(char *line, WebServer &webServer);

public:
    ParseConfig(std::string);
    std::vector<WebServer>  parse();
    void printWebservers();
};

#endif