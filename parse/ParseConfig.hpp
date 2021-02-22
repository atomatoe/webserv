#ifndef PARSECONFIG_HPP
#define PARSECONFIG_HPP

#include "../includes/includes.hpp"
#include "../WebServ.hpp"

void    exitError(std::string);
bool    isDirectory(std::string path);
bool    isFileRead(std::string path);
bool    isFileExec(std::string path);

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

    /* check location params */
    bool            checkRootLoc(char *line, Location &location);
    bool            checkIndex(char *line, Location &location);
    bool            checkCgiPass(char *line, Location &location);
    bool            checkAutoindex(char *line, Location &location);
    bool            checkLimitBody(char *line, Location &location);
    bool            checkAllowMethods(char *line, Location &location);

    /* parse algorithm */
    void            parseServer(char *line);
    std::string     parseLocation(char *line, WebServer &webServer);

public:
    ParseConfig(std::string);
    std::vector<WebServer>  parse();
    void printWebservers();
};

#endif