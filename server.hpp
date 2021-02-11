/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 13:23:07 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/10 15:44:51 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "./includes/includes.hpp"
#include "request/Request.hpp"
#include "cgi_proccesing/cgi.h"

typedef struct client 
{ 
    char *buff_read; // то что запросит клиент
    char *buff_write; // то что отдадим клиенту
} t_client;

class Server
{
    private:
        fd_set                  fd_write;
        fd_set                  fd_read;
        fd_set                  fd_write_tmp; // fd_write_tmp - нужен для работы select. При вызове макроса FD_ISSET портится fdшник, поэтому мы будем делать копию в fd_read_tmp, чтобы портилась она
        fd_set                  fd_read_tmp; // fd_read_tmp - нужен для работы select. При вызове макроса FD_ISSET портится fdшник, поэтому мы будем делать копию в fd_read_tmp, чтобы портилась она
        std::map<int, t_client> list; // здесь будут храниться наши клиенты
        sockaddr_in             socket_addr; // стандартная структура (см в конце)
        socklen_t               address_len; // Аргумент address_len задает размер (в байтах) структуры данных, указываемой аргументом addr.
		sockaddr_in		        out; // out - заполнит значениями клиента - пример принятия запроса
        int                     server_fd; // основной fd где открыт сервер
        int                     yes; // не знаю что это но это работает
        int                     max_fd;
    public:
        // construct:
        Server();
        ~Server();
        // func:
        int create_server();
        int start_server();
        void set_socket(char *ip, int port);
        // get
        int get_server_fd();
        int get_max_fd();
        fd_set *get_fd_read();
        fd_set *get_fd_read_tmp();
        fd_set *get_fd_write();
        fd_set *get_fd_write_tmp();
        std::map<int, t_client> &get_list();
        sockaddr_in *get_socket_addr();
        socklen_t *get_address_len();
        sockaddr_in *get_out();
        // set
        void set_max_fd(int count);
        void set_fd_read(fd_set fd);
        void set_fd_read_tmp(fd_set fd);
        void set_fd_write(fd_set fd);
        void set_fd_write_tmp(fd_set fd);
};

#endif


/*
struct sockaddr_in 
{ 
    __uint8_t         sin_len;  // длина в байтах структуры
    sa_family_t       sin_family;  // флаг передачи между сокетами (интернет/локалка)
    in_port_t         sin_port;  // порт
    struct in_addr    sin_addr;  // структура в которой лежит ip adress
    char              sin_zero[8]; 
};
*/