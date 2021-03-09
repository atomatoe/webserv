//
// Created by Wynn Elease on 2/21/21.
//

#ifndef WEBSERV_CGI_UTILS_H
#define WEBSERV_CGI_UTILS_H

#include <fcntl.h>
class Response;
#include "../Response/Response.hpp"
#include "../WebServer/WebServ.hpp"
#include <iostream>
const int envSize = 20;
const int argvSize = 3;
const size_t bufSize = 100000200;
void toCGI(Response &response, Request & request, WebServer & server, std::string & directory);

#endif
