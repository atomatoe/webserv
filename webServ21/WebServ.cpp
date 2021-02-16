/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 14:02:33 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/16 16:56:57 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

WebServer::WebServer(std::string ip, int port, std::string serverName, std::string rootPath, std::vector<Location> locations)
{
    this->_ip = ip;
    this->_port = port;
    this->_serverName = serverName;
    this->_rootPath = rootPath;
    this->_locations = locations;
    //
    this->_yes = 1;
    this->_server_fd = 0;
}

WebServer::~WebServer()
{
    
}

void WebServer::create_socket()
{
    this->_server_fd = socket(AF_INET, SOCK_STREAM, 0);
    this->_address_len = (sizeof(this->_socket_addr));
    this->_socket_addr.sin_family = AF_INET;
    this->_socket_addr.sin_addr.s_addr = inet_addr(this->_ip.c_str()); // содержит адрес (номер) узла сети.
    this->_socket_addr.sin_port = htons(this->_port); // 80 - port с конфига  что делает htons (80 >> 8 | 80 << 8)
}

int WebServer::build_server()
{
    if(setsockopt(this->_server_fd, SOL_SOCKET, SO_REUSEADDR, &this->_yes, sizeof(int))) // дает возможность повторно использовать сокет (повторять bind)
    {
        std::cerr << "setsockopt FAILED" << std::endl;
        return(-1);
    }
    if(bind(this->_server_fd,(const struct sockaddr *) &this->_socket_addr, this->_address_len)) // Аргумент address_len задает размер (в байтах) структуры данных, указываемой аргументом addr.
    {
        std::cerr << "bind FAILED" << std::endl;
        return(-1);
    }
    if(fcntl(this->_server_fd, F_SETFL, O_NONBLOCK)) // делаем сокет неблокирующемся
    {
        std::cerr << "fcntl FAILED" << std::endl;
        return(-1);
    }
    if(listen(this->_server_fd, 5))
    {
        std::cerr << "listen FAILED" << std::endl;
        return(-1);
    }
    return(0);
}

int WebServer::get_server_fd() { return(this->_server_fd); }
sockaddr_in *WebServer::get_socket_addr() { return(&this->_socket_addr); }
socklen_t *WebServer::get_address_len() { return(&this->_address_len); }
sockaddr_in *WebServer::get_out() { return(&this->_out); }
std::map<int, t_client> &WebServer::get_list() { return(this->_list); }