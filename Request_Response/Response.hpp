/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 16:10:50 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/18 15:02:48 by atomatoe         ###   ########.fr       */
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
	Bytes		_bodyOfResponse;
	size_t 		_lenOfResponse;

	int         _location_id;
public:
	Response();
	~Response();
	char* give_me_response(Request request, WebServer server);
	char* edit_response();
	int search_uri(WebServer server, char *uri);
	int check_uri(WebServer server, char *uri);
	void putErrorToBody(char *error, char *type);

	size_t getLenOfResponse() const;

	friend void toCGI(Response &response, Request request, WebServer server);

};
#endif