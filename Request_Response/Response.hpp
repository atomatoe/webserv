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

#include "../includes/includes.hpp"
#include "Request.hpp"

class Response
{
private:
    std::string _h1;
    std::string _h2;
    std::string _h3;
    std::string _body;
    char *      _response;

    int         _location_id;
public:
    Response();
    ~Response();
    char* give_me_response(Request request, WebServer server);
    char* edit_response();
    int search_uri(WebServer server, char *uri);
    int check_uri(WebServer server, char *uri);

};
#endif