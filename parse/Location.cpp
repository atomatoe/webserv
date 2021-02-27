#include "Location.hpp"

Location::Location() {
    _allowMethods.insert(std::pair<std::string, bool>("GET", false));
    _allowMethods.insert(std::pair<std::string, bool>("POST", false));
    _allowMethods.insert(std::pair<std::string, bool>("PUT", false));
    _allowMethods.insert(std::pair<std::string, bool>("HEAD", false));
    _limitBody = -1;
}

/* get */
const std::string &                         Location::getUrl() const { return _url; }
const std::string &                         Location::getRoot() const { return _root; }
const std::map<std::string, std::string> &  Location::getCgiPath() const { return _cgiPath; }
int                                         Location::getLimitBody() const { return _limitBody; }
const std::map<std::string, bool> &         Location::getAllowMethods() const { return _allowMethods; }
const std::string &                         Location::getIndex() const { return _index; }

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
void                                        Location::addCgiPath(std::string error, std::string path) {
    if (error[0] != '.')
        exitError("Smth wrong with cgi_pass value: \"" + error + "\"");
    if (!isFileExec(path))
        exitError("Smth wrong with cgi_pass: \"" + path + "\"");
    _cgiPath.insert(std::pair<std::string, std::string>(error, path));
}
void                                        Location::changeAllowMethod(std::string key, bool value) {
    _allowMethods[key] = value;
}

