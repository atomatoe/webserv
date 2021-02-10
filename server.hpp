/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 13:23:07 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/03 15:36:40 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "includes.hpp"

typedef struct client 
{ 
    char *buff_read; // то что запросит клиент
    char *buff_write; // то что отдадим клиенту
} t_client;

class Server
{
    private:
        fd_set fd_read;
        fd_set fd_read_tmp; // fd_read_tmp - нужен для работы select. При вызове макроса FD_ISSET портится fdшник, поэтому мы будем делать копию в fd_read_tmp, чтобы портилась она
        fd_set fd_write;
        fd_set fd_write_tmp; // fd_write_tmp - нужен для работы select. При вызове макроса FD_ISSET портится fdшник, поэтому мы будем делать копию в fd_read_tmp, чтобы портилась она
        std::map<int, t_client> list; // здесь будут храниться наши клиенты
        sockaddr_in     socket_addr; // стандартная структура (см в конце)
        socklen_t       address_len; // Аргумент address_len задает размер (в байтах) структуры данных, указываемой аргументом addr.
		sockaddr_in		out; // out - заполнит значениями клиента - пример принятия запроса
        int server_fd; // основной fd где открыт сервер
        int max_fd;
    public:
        Server()
        {
            max_fd = 0;
            FD_ZERO(&fd_write); // зануление fd
            FD_ZERO(&fd_read); // зануление fd
            FD_ZERO(&fd_write_tmp); // зануление fd
            FD_ZERO(&fd_read_tmp); // зануление fd
            server_fd = socket(AF_INET, SOCK_STREAM, 0);
        }
        ~Server() { close(server_fd); };
        int get_server_fd() { return(server_fd); }
        int get_max_fd() { return(max_fd); }
        void set_max_fd(int count) { max_fd = count; }
        fd_set *get_fd_read() { return(&fd_read); }
        fd_set *get_fd_read_tmp() { return(&fd_read_tmp); }
        fd_set *get_fd_write() { return(&fd_write); }
        fd_set *get_fd_write_tmp() { return(&fd_write_tmp); }
        void set_fd_read(fd_set fd) { fd_read = fd; }
        void set_fd_read_tmp(fd_set fd) { fd_read_tmp = fd; }
        void set_fd_write(fd_set fd) { fd_write = fd; }
        void set_fd_write_tmp(fd_set fd) { fd_write_tmp = fd; }
        std::map<int, t_client> &get_list() { return(list); }
        void set_socket(char *ip, int port)
        {
            address_len = (sizeof(socket_addr));
            socket_addr.sin_family = AF_INET;
            socket_addr.sin_addr.s_addr = inet_addr(ip); // содержит адрес (номер) узла сети.
            socket_addr.sin_port = htons(port); // 80 - port с конфига  что делает htons (80 >> 8 | 80 << 8)
        }
        sockaddr_in *get_socket_addr() { return(&socket_addr); }
        socklen_t *get_address_len() { return(&address_len); }
        sockaddr_in *get_out() { return(&out); }
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