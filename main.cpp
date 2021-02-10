/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 13:26:03 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/10 15:52:09 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
// nc -c 127.0.0.1 8080 - подключение через терминал

int main(int argc, char **argv)
{
    // if(argc != 2)
    //     std::cout << "Неверное количество аргументов" << std::endl;
    // else if(ft_parser_config_file() == 0)
    //     return (-1);

    // -------------------------------------
    std::vector<Server> servers;
    Server server;
    server.set_socket("127.0.0.1", 8080);
    servers.push_back(server);
    // -------------------------------------
    size_t it = 0;
    size_t count_server = 1;
    for(size_t it = 0; it != count_server; it++)
    {
        if(servers[it].create_server() == 5)
            exit(1);
        if(servers[it].start_server() == 5)
            exit(1);
    }
    return (0);
}


// write(it->first , hello , strlen(hello));
// send(it->first, it->second.buff_write, strlen(it->second.buff_write), 0);
// что лучше?