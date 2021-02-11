//
// Created by Wynn Elease on 2/11/21.
//

#ifndef WEBSERV_CGI_H
#define WEBSERV_CGI_H
#include <fcntl.h>
#include "Request.hpp"

int toCGI(Request request, char** buffWriteFd);
int getEnv(char **env, Request request);

#endif //WEBSERV_CGI_H
