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
};

#endif