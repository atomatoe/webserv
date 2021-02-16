/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 18:11:54 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/16 18:28:12 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_REQUEST_H
#define WEBSERV_REQUEST_H
#include <map>
#include <iostream>
#include "../libft/libft.h"
#include <cmath>

class Request{
public:
   Request(char *reqString, int fd);
   ~Request();
   char *getMetod() {return _info["metod"];};
   char *getURI() {return _info["uri"];};
   char *getContentType() {return _info["Content-Type"];}
   char *getContentLength() {return _info["Content-Length"];}
   char *getReqString() {return _reqString;}
   char *getTransferEncoding() {return _info["Transfer-Encoding"];}
private:
   char *_reqString;
   char *_reqBody;
   int    _fd;
   std::map<std::string,char*> _info;
   int parsRequest(char *reqString);
   int parsFirstLine(char **copy);
   void fillMap();
   int parsHeaders(char **strings);
   char *putChunkedReqBody();
};
#endif