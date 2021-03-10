/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 16:10:50 by atomatoe          #+#    #+#             */
/*   Updated: 2021/03/08 19:48:32 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../Bytes/Bytes.hpp"
class Request;
#include "../Request/Request.hpp"
#include "../CGI/utilsOfCGI.hpp"

class Response {

private:
	std::string 			_httpVersion;
	std::string 			_timeOfResponse;
	std::string 			_versionOfWebServer;
	std::string 			_contentLength;
	Bytes					_bodyOfResponse;
	size_t 					_lenOfResponse;
	int         			_location_id;
	static const size_t 	bufSize = 256000;

public:
	Response();
	~Response();
	char* 			responseGenerating(Request & request, WebServer & server);
	char* 			editResponse();
	int 			uriSearching(WebServer & server, char *uri);
	void 			putErrorToBody(char *error, char *type, WebServer server);

	void 			methodGetHead(Request & request, WebServer & server, Page_html & page);
	void 			methodPost(Request & request, WebServer & server);
	void 			methodPut(Request & request, WebServer & server);
	void 			checkFileOrDir(Request & request, WebServer & server);
	std::string 	indexSearching(std::string index);
	Bytes & 		getBodyOfResponse();
	size_t 			getLenOfResponse() const;

	friend void 	toCGI(Response &response, Request & request, WebServer & server);
	int 			check_auth(Request & request, Location & location);
};
#endif