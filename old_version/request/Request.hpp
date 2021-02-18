//
// Created by Wynn Elease on 2/4/21.
//

#ifndef WEBSERV_REQUEST_H
#define WEBSERV_REQUEST_H
#include <map>
#include <iostream>
#include "../libft/libft.h"

class Request{

public:
	Request(char *reqString);
	~Request();
	char *getMetod() {return _info["metod"];};
	char *getURI() {return _info["uri"];};
	char *getContentType() {return _info["Content-Type"];}
	char *getContentLength() {return _info["Content-Length"];}
	char *getReqString() {return _reqString;}
private:
	char *_reqString;
	std::map<std::string,char*> _info;
	int parsRequest(char *reqString);
	int parsFirstLine(char **copy);
	void fillMap();
	int parsHeaders(char **strings);
};

#endif
