/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 18:11:54 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/28 18:34:33 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_REQUEST_H
#define WEBSERV_REQUEST_H
#include <map>
#include <iostream>
#include "Bytes.h"
#include <cmath>
#include <sstream>
#include <iomanip>
#include "../includes/includes.hpp"
#include "Response.hpp"
#include "HeaderValidation.hpp"

class Request{
public:
	Request(char *reqString);
	~Request();

	void 			setReqBody(char *body, size_t len);
	char 			*getMetod() {return _info["metod"];};
	char 			*getURI() {return _info["uri"];};
	char 			*getContentType() {return _info["Content-Type"];}
	char 			*getContentLength() {return _info["content-length"];}
	char			*getTransferEncoding() {return _info["Transfer-Encoding"];}
	char			*getAuthorization() {return _info["Authorization"];}
	char 			*getReqString() {return _reqString;}
	Bytes 			& getReqBody() {return _reqBody;}
	std::string &	getPathToCgi();

	std::string &	getQueryString();

	void			setPathToCgi(const std::string &pathToCgi);

	void			ChunkedBodyProcessing();

private:
	char                        *_reqString;
	Bytes                       _reqBody;
	Bytes                       _chunkedReqBody;
	std::map<std::string,char*> _info;
    HeaderValidation            _headValid;
    std::string					_pathToCgi;
    std::string					_queryString;

    int parsRequest(char *reqString);
    int parsFirstLine(char **copy);
    void fillMap();

	int parsHeaders(char **strings);
};
#endif