#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "../../Includes/Includes.hpp"

class Location {
private:
    std::string                         _url;
    std::string                         _root;
    std::string                         _index;
    std::map<std::string, std::string>  _cgiPath;
    int 		                        _limitBody;
    std::map<std::string, bool>         _allowMethods;

public:
    Location();

    /* get */
    const std::string &                         getUrl() const;
    const std::string &                         getRoot() const;
    const std::string &                         getIndex() const;
    const std::map<std::string, std::string> &  getCgiPath() const;
    int                                         getLimitBody() const;
    const std::map<std::string, bool> &         getAllowMethods() const;

    /* set */
    void                                setUrl(const std::string &);
    void                                setRoot(const std::string &);
    void                                setIndex(const std::string &);
    void                                addCgiPath(std::string, std::string);
    void                                setLimitBody(int);
    void                                changeAllowMethod(std::string, bool);

};

#endif