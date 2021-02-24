//
// Created by Wynn Elease on 2/21/21.
//

#ifndef WEBSERV_CGI_UTILS_H
#define WEBSERV_CGI_UTILS_H

#include <fcntl.h>
class Response;
#include "Response.hpp"

void toCGI(Response &response, Request request, WebServer server);

#endif //WEBSERV_CGI_UTILS_H
