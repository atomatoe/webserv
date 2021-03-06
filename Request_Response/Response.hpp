/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 16:10:50 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/27 21:24:48 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

//#include "../includes/includes.hpp"
#include "Bytes.h"
class Request;
#include "Request.hpp"
#include "../cgi_processing/cgi_utils.h"

class Response
{
private:
	std::string _httpVersion;
	std::string _timeOfResponse;
	std::string _versionOfWebServer;
	std::string _contentLength;
	Bytes		_bodyOfResponse;
	size_t 		_lenOfResponse;

	int         _location_id;
public:
	Response();
	~Response();
	char* give_me_response(Request request, WebServer & server);
	char* edit_response(Request *request);
	int search_uri(WebServer & server, char *uri);
	void putErrorToBody(char *error, char *type, WebServer server);
	
	void methodGetHead(Request & request, WebServer & server, Page_html & page);
	void methodPost(Request & request, WebServer & server, Page_html & page);
	void methodPut(Request & request, WebServer & server, Page_html & page);
	void check_file_or_dir(Request & request, WebServer & server);
	std::string give_me_index(std::string index);

	size_t getLenOfResponse() const;

	friend void toCGI(Response &response, Request request, WebServer server);
};
#endif