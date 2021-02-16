/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 13:23:07 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/16 15:42:02 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "./includes/includes.hpp"
#include "./includes/Location.hpp"

typedef struct client 
{ 
    char *buff_read; // то что запросит клиент
    char *buff_write; // то что отдадим клиенту
} t_client;

class WebServer
{
private:
	std::string _ip;
	int			_port;
	std::string _serverName;
	std::string _rootPath;
	std::vector<Location> _locations;
	//
	std::map<int, t_client> _list; // здесь будут храниться наши клиенты
    sockaddr_in             _socket_addr; // стандартная структура (см в конце)
    socklen_t               _address_len; // Аргумент address_len задает размер (в байтах) структуры данных, указываемой аргументом addr.
	sockaddr_in		        _out; // out - заполнит значениями клиента - пример принятия запроса
    int                     _server_fd; // основной fd где открыт сервер
    int                     _yes; // не знаю что это но это работает
public:
	WebServer(std::string ip, int port, std::string serverName, std::string rootPath, std::vector<Location> locations);
	~WebServer();
	void create_socket();
	int build_server();
	//geters
	int get_server_fd();
	sockaddr_in *get_socket_addr();
    socklen_t *get_address_len();
    sockaddr_in *get_out();
	std::map<int, t_client> &get_list();
};

#endif