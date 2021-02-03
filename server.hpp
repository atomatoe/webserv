/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 13:23:07 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/03 13:26:30 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "includes.hpp"

class Server
{
    private:
        fd_set fd_read;
        fd_set fd_read_tmp;
        fd_set fd_write;
        fd_set fd_write_tmp;
        int server_fd;
    public:
        Server()
        {
            FD_ZERO(&fd_write);
            FD_ZERO(&fd_read);
            server_fd = socket(AF_INET, SOCK_STREAM, 0);
            // FD_ZERO(&fd_write_tmp);
            // FD_ZERO(&fd_read_tmp);
        }
        ~Server() {};
};

#endif