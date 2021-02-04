/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 13:26:03 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/04 15:45:19 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "includes.hpp"
#include "socket.hpp"
#include "error_handler.hpp"


// nc -c 127.0.0.1 8080 - подключение через терминал

int main(int argc, char **argv)
{
    // if(argc != 2)
    //     std::cout << "Неверное количество аргументов" << std::endl;
    // else if(ft_parser_config_file() == 0)
    //     return (-1);

    Server server;
    Socket socket("127.0.0.1", 8080);
    Error_handler error404("404", "Page not found.");
    
    int yes = 1; // не знаю что это но это работает
    if(setsockopt(server.get_server_fd(), SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))) // дает возможность повторно использовать сокет (повторять bind)
    {
        std::cerr << "setsockopt FAILED" << std::endl;
        exit(1);
    }
    if(bind (server.get_server_fd(), (const struct sockaddr *) socket.get_socket_addr(), *(socket.get_address_len()))) // Аргумент address_len задает размер (в байтах) структуры данных, указываемой аргументом addr.
    {
        std::cerr << "bind FAILED" << std::endl;
        exit(1);
    }
    if(fcntl(server.get_server_fd(), F_SETFL, O_NONBLOCK)) // делаем сокет неблокирующемся
    {
        std::cerr << "fcntl FAILED" << std::endl;
        exit(1);
    }
    if(listen(server.get_server_fd(), 5))
    {
        std::cerr << "listen FAILED" << std::endl;
        exit(1);
    }
    FD_SET(server.get_server_fd(), server.get_fd_read()); // заносим fd сокета(то есть сервера) в список fdшников
    FD_SET(server.get_server_fd(), server.get_fd_write()); // заносим fd сокета(то есть сервера) в список fdшников
    server.set_max_fd(server.get_server_fd()); // потом нужно найти самый большой fdшник и передевать его первым аргументом в select.
    while(1)
    {
        server.set_fd_read_tmp(*(server.get_fd_read()));
        server.set_fd_write_tmp(*(server.get_fd_write()));
        select(server.get_max_fd() + 1, server.get_fd_read_tmp(), server.get_fd_write_tmp(), NULL, NULL);
        if(FD_ISSET(server.get_server_fd(), server.get_fd_read_tmp())) // 1 блок = проверка на пришел кто то или нет
        {
            int fd = accept(server.get_server_fd(), (struct sockaddr *) socket.get_out(), socket.get_address_len()); // fd - новый клиент
            // out - заполнит значениями клиента - пример принятия запроса
            if(fd > 0)
            {
                FD_SET(fd, server.get_fd_read()); // заносим клиента в fd для чтения
                FD_SET(fd, server.get_fd_write()); // заносим клиента в fd для записи
                if(fd > server.get_max_fd())
                    server.set_max_fd(fd);
                (server.get_list())[fd].buff_read = 0;
                (server.get_list())[fd].buff_write = 0;
            }
            // send(fd, "hello world!\n", 20, 0); // пример ответа клиенту
        }
        for(std::map<int, t_client>::iterator it = (server.get_list()).begin(); it != (server.get_list()).end(); it++) // 2 блок = на получение данных (отправил или нет)
        {
            if(FD_ISSET(it->first, server.get_fd_read_tmp()))
            {
                char* newbuf = (char *)malloc(sizeof(char) * 4097);
                int ret = recv(it->first, newbuf, 4096, 0);
                if(ret == 0)
                {
                    close(it->first);
                    if(it->second.buff_read)
                        free(it->second.buff_read);
                    if(it->second.buff_write)
                        free(it->second.buff_write);
                    FD_CLR(it->first, server.get_fd_read());
                    FD_CLR(it->first, server.get_fd_write());
                    server.get_list().erase(it);
                    free(newbuf);
                    break ; // делаем break чтобы не наебнулась it++ 
                }
                else if(ret > 0)
                {
                    newbuf[ret] = '\0';
                    it->second.buff_read = str_join(it->second.buff_read, newbuf);
                    std::cout << "запрос: " << it->second.buff_read << std::endl;
                    
                    it->second.buff_write = str_join(it->second.buff_write, "HTTP/1.1 200 OK\nDate: Mon, 27 Jul 2009 12:28:53 GMT\nServer: Apache/2.2.14 (Win32)\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\nContent-Type: text/html\nConnection: Closed\n\n");
                    // вот здесь нужно сформировать ответ клиенту в it->second.buff_write
                }
                free(newbuf);
            }
        }
        for(std::map<int, t_client>::iterator it = (server.get_list()).begin(); it != (server.get_list()).end(); it++) // 3 блок готов ли кто-то получить от тебя данные или нет
        {
            if(FD_ISSET(it->first, server.get_fd_write_tmp()))
            {
                if(it->second.buff_write)
                {
                    int fd = open("index.html", O_RDONLY);
                    char buff[5000];
                    char* hello = (char *)malloc(sizeof(char) * 4097);
                    int i = read(fd, hello, 5555);
                    // char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
                    // вот здесь прикручиваем в первой части ответа html страницу, если она нужна
                    // char *ht = error404.create_error(); // создание ошибки
                    it->second.buff_write = str_join(it->second.buff_write, hello);
                    int ret = send(it->first, it->second.buff_write, strlen(it->second.buff_write), 0);
                    if(ret != strlen(it->second.buff_write))
                    {
                        char *newbuf = NULL;
                        newbuf = str_join(newbuf, it->second.buff_write + ret);
                        free(it->second.buff_write);
                        it->second.buff_write = newbuf;
                    }
                    else
                    {
                        free(it->second.buff_write);
                        it->second.buff_write = NULL;
                    }
                }
            }
        }
    }
    return (0);
}


// write(it->first , hello , strlen(hello));
// send(it->first, it->second.buff_write, strlen(it->second.buff_write), 0);
// что лучше?