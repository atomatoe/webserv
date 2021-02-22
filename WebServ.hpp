/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 13:23:07 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/17 19:03:30 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

//#include "includes/includes.hpp"
#include "Request_Response/Bytes.h"
#include <vector>
#include "Request_Response/Response.hpp"
#include "Request_Response/Request.hpp"
class Response;

typedef struct client {
	Request *request;
	Response *response;
	Bytes *receivedData;// то что запросит клиент
	Bytes *toSendData; // то что отдадим клиенту
	int isHeadersEnded;
} t_client;

class WebServer
{
private:
	std::string                         _ip;
	int		                            _port;
	std::string                         _serverName;
	std::string                         _rootPath;
	std::vector<Location>               _locations;
	std::map<std::string, std::string>  _errorPage;
	//
	std::map<int, t_client> _list; // здесь будут храниться наши клиенты
    sockaddr_in             _socket_addr; // стандартная структура (см в конце)
    socklen_t               _address_len; // Аргумент address_len задает размер (в байтах) структуры данных, указываемой аргументом addr.
	sockaddr_in		        _out; // out - заполнит значениями клиента - пример принятия запроса
    int                     _server_fd; // основной fd где открыт сервер
    int                     _yes; // не знаю что это но это работает
public:
	WebServer();
	~WebServer();
	void create_socket();
	int build_server();
	//geters
	int get_server_fd();
	sockaddr_in *get_socket_addr();
    socklen_t *get_address_len();
    sockaddr_in *get_out();
	std::map<int, t_client> &get_list();

    /* get */
    std::string                         getIp() const;
    int                                 getPort() const;
    std::string                         getServerName() const;
    std::string                         getRootPath() const;
    std::map<std::string, std::string>  getErrorPage() const;
    std::vector<Location>               getLocations() const;

    /* set */
    void    setIp(std::string);
    void    setPort(int port);
    void    setServerName(std::string);
    void    setRootPath(std::string);

    /* add */
    void    addErrorPage(std::string, std::string);
    void    addLocation(Location location);
};

#endif