//
// Created by Wynn Elease on 2/21/21.
//

#ifndef WEBSERV_CGI_UTILS_H
#define WEBSERV_CGI_UTILS_H

#include <fcntl.h>
#include "Request.hpp"

int toCGI(Request request, char** buffWriteFd);
int getEnv(char **env, Request request);

#endif //WEBSERV_CGI_UTILS_H
