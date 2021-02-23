//
// Created by Quaro Tamarillo on 2/23/21.
//

#include "includes/includes.hpp"

void    errorMalloc() {
    std::cerr << "Malloc error" << std::endl;
    exit(1);
}

void	exitSignal(int signalNum) {
    (void)signalNum;
    std::cout << GREEN << "Webserv has finished working" << DEFAULT<< std::endl;
    exit(0);
}

void init_servers(std::vector<WebServer> & servers, size_t count_server)
{
    size_t i = 0;
    while(i != count_server)
    {
        servers[i].create_socket();
        servers[i].build_server();
        i++;
    }
}
