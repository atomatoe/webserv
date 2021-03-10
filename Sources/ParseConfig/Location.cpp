#include "Location.hpp"

Location::Location() {
    _allowMethods.insert(std::pair<std::string, bool>("GET", false));
    _allowMethods.insert(std::pair<std::string, bool>("POST", false));
    _allowMethods.insert(std::pair<std::string, bool>("PUT", false));
    _allowMethods.insert(std::pair<std::string, bool>("HEAD", false));
    _limitBody = -1;
}

/* get */
const std::string &                                                 Location::getUrl() const { return _url; }
const std::string &                                                 Location::getRoot() const { return _root; }
const std::map<std::string, std::pair<std::string,std::string> > &   Location::getCgiPath() const { return _cgiPath; }
int                                                                 Location::getLimitBody() const { return _limitBody; }
const std::map<std::string, bool> &                                 Location::getAllowMethods() const { return _allowMethods; }
const std::string &                                                 Location::getIndex() const { return _index; }
std::vector<std::string>                                            Location::getAuthClients() const { return _auth_client; }

/* set */
void                                        Location::setUrl(const std::string &url) {
    if (url[0] != '.' && url[0] != '/')
        exitError("Location url is incorrect");
    _url = url;
}
void                                        Location::setRoot(const std::string &rootPath) {
    if (!isDirectory(rootPath))
        exitError("Smth wrong with root value: \"" + rootPath + "\"");
    _root = rootPath;
}
void                                        Location::setLimitBody(int limitBody) {
    _limitBody = limitBody;
}
void                                        Location::setIndex(const std::string &index) {
    if (_root.empty())
        exitError("Index should be with root");
    if (index[0] == '/' || !isFileRead(_root + "/" + index))
        exitError("Incorrect value in index \"" + index + "\"");
    _index = _root + "/" + index;
}
void                                        Location::addCgiPath(std::string extension, std::string pathInterpreter, std::string path) {
    if (extension[0] != '.')
        exitError("Smth wrong with cgi_pass value: \"" + extension + "\"");
    if (!isFileExec(path))
        exitError("Smth wrong with cgi_pass: \"" + path + "\"");
    if (pathInterpreter != "") {
        if (!isFileExec(pathInterpreter))
            exitError("Smth wrong with cgi_pass: \"" + pathInterpreter + "\"");
        _cgiPath.insert(std::pair<std::string, std::pair<std::string,std::string> >(extension, std::pair<std::string, std::string>(pathInterpreter, path)));
    }
    _cgiPath.insert(std::pair<std::string, std::pair<std::string,std::string> >(extension, std::pair<std::string, std::string>("", path)));
}
void                                        Location::changeAllowMethod(std::string key, bool value) {
    _allowMethods[key] = value;
}

void                                        Location::addAuthClient(const std::string &file) {
    int         fd;
    char        *line;

    fd = 0;
    if (!isFileRead(file) || (fd = open(file.c_str(), O_RDONLY)) < 0)
        exitError("Smth wrong with auth_clients: \"" + file + "\"");
    while (get_next_line(fd, &line)) {
        std::string     lineStr(line);
        std::vector<std::string> spliited = ft_splitString(line, ":");
        if (!ft_isalnum(line[0]) || !ft_isalnum(line[ft_strlen(line) - 1]) || spliited.size() != 2 || lineStr.find(' ') != std::string::npos) {
            free(line);
            close(fd);
            exitError("Incorrect value of auth_clients: \"" + lineStr + "\"");
        }
        _auth_client.push_back(lineStr);
        free(line);
    }
    free(line);
    close(fd);
}

bool					operator==(const Location& location1, const Location& location2) {
    if (location1.getUrl() == location2.getUrl())
        return true;
    return false;
}
