/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 18:11:54 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/18 16:15:24 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_REQUEST_H
#define WEBSERV_REQUEST_H
#include <map>
#include <iostream>
#include "../Bytes/Bytes.hpp"
#include <cmath>
#include <sstream>
#include <iomanip>
#include "../../Includes/Includes.hpp"
#include "../Response/Response.hpp"
#include "../ValidationOfRequest/HeaderValidation.hpp"

class Request {

public:
	Request(char *reqString);
	~Request();

	std::string 				getMetod() {return _info["metod"];};
	std::string 				getURI() {return _info["uri"];};
	std::string 				getContentType() {return _info["Content-Type"];}
	std::string 				getContentLength() {return _info["content-length"];}
	std::string					getTransferEncoding() {return _info["Transfer-Encoding"];}
	Bytes 		& 				getReqBody() {return _reqBody;}
	std::string &				getPathToCgi();
	std::string &				getQueryString();
	bool 						isHeadersParsed() ;
	void						setPathToCgi(const std::string &pathToCgi);
	void						ChunkedBodyProcessing();

private:
	char                        *_reqString;
	Bytes                       _reqBody;
	Bytes                       _chunkedReqBody;
	std::map<std::string,std::string> _info;
    HeaderValidation            _headValid;
    std::string					_pathToCgi;
    std::string					_queryString;
    bool 						_parsedHeaders;

    int 						parsRequest(char *reqString);
    int 						parsFirstLine(char **copy);
    void 						checkEndOfBody();
    void 						fillMap();
	int 						parsHeaders(char **strings);
};
#endif