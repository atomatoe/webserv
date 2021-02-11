/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 15:09:20 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/11 13:02:02 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/page_html.hpp"
#include "server.hpp"

Server::Server()
{
    max_fd = 0;
    yes = 1;
    FD_ZERO(&fd_write); // зануление fd
    FD_ZERO(&fd_read); // зануление fd
    FD_ZERO(&fd_write_tmp); // зануление fd
    FD_ZERO(&fd_read_tmp); // зануление fd
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
}

Server::~Server()
{
    close(server_fd);
}

int Server::create_server()
{
    size_t it = 0;
    if(setsockopt(get_server_fd(), SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))) // дает возможность повторно использовать сокет (повторять bind)
    {
        std::cerr << "setsockopt FAILED" << std::endl; return(5); }
    if(bind (get_server_fd(), (const struct sockaddr *) get_socket_addr(), *(get_address_len()))) // Аргумент address_len задает размер (в байтах) структуры данных, указываемой аргументом addr.
    {
        std::cerr << "bind FAILED" << std::endl; return(5); }
    if(fcntl(get_server_fd(), F_SETFL, O_NONBLOCK)) // делаем сокет неблокирующемся
    {
        std::cerr << "fcntl FAILED" << std::endl; return(5); }
    if(listen(get_server_fd(), 5))
    {
        std::cerr << "listen FAILED" << std::endl; return(5); }
    FD_SET(get_server_fd(), get_fd_read()); // заносим fd сокета(то есть сервера) в список fdшников
    FD_SET(get_server_fd(), get_fd_write()); // заносим fd сокета(то есть сервера) в список fdшников
    set_max_fd(get_server_fd()); // потом нужно найти самый большой fdшник и передевать его первым аргументом в select.
    return(0);
}
int Server::start_server()
{
    Page_html test;
    while(1)
    {
        set_fd_read_tmp(*(get_fd_read()));
        set_fd_write_tmp(*(get_fd_write()));
        select(get_max_fd() + 1, get_fd_read_tmp(), get_fd_write_tmp(), NULL, NULL);
        if(FD_ISSET(get_server_fd(), get_fd_read_tmp())) // 1 блок = проверка на пришел кто то или нет
        {
            int fd = accept(get_server_fd(), (struct sockaddr *) get_out(), get_address_len()); // fd - новый клиент
            // out - заполнит значениями клиента - пример принятия запроса
            if(fd > 0)
            {
                FD_SET(fd, get_fd_read()); // заносим клиента в fd для чтения
                FD_SET(fd, get_fd_write()); // заносим клиента в fd для записи
                if(fd > get_max_fd())
                    set_max_fd(fd);
                (get_list())[fd].buff_read = 0;
                (get_list())[fd].buff_write = 0;
            }
            // send(fd, "hello world!\n", 20, 0); // пример ответа клиенту
        }
        for(std::map<int, t_client>::iterator it = (get_list()).begin(); it != (get_list()).end(); it++) // 2 блок = на получение данных (отправил или нет)
        {
            if(FD_ISSET(it->first, get_fd_read_tmp()))
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
                    FD_CLR(it->first, get_fd_read());
                    FD_CLR(it->first, get_fd_write());
                    get_list().erase(it);
                    free(newbuf);
                    break ; // делаем break чтобы не наебнулась it++ 
                }
                else if(ret > 0)
                {
                    newbuf[ret] = '\0';
                    it->second.buff_read = str_join(it->second.buff_read, newbuf);
					Request request(it->second.buff_read);
					it->second.buff_write = ft_strdup("HTTP/1.1 200 OK\nConnection: Closed");
					//it->second.buff_write = str_join(it->second.buff_write, "HTTP/1.1 200 OK\nDate: Mon, 27 Jul 2009 12:28:53 GMT\nServer: Apache/2.2.14 (Win32)\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\nConnection: Closed");
					//std::cout << "|" << it->second.buff_write << "|" << std::endl << std::endl << std::endl;
					toCGI(request, &(it->second.buff_write)); //todo error handler
                  // 	std::cout << it->second.buff_read << std::endl;
                  //  it->second.buff_write = str_join(it->second.buff_write, (char *)"HTTP/1.1 200 OK\nDate: Mon, 27 Jul 2009 12:28:53 GMT\nServer: Apache/2.2.14 (Win32)\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\nContent-Type: text/html\nConnection: Closed\n\n");
                    // вот здесь нужно сформировать ответ клиенту в it->second.buff_write
                }
                free(newbuf);
            }
        }
        for(std::map<int, t_client>::iterator it = (get_list()).begin(); it != (get_list()).end(); it++) // 3 блок готов ли кто-то получить от тебя данные или нет
        {
            if(FD_ISSET(it->first, get_fd_write_tmp()))
            {
                if(it->second.buff_write)
                {
                    //int fd = open((char *)"index.html", O_RDONLY);
                    //char buff[5000];
                    //char* hello = (char *)malloc(sizeof(char) * 4097);
                    //read(fd, hello, 5555);
                    // вот здесь прикручиваем в первой части ответа html страницу, если она нужна
                   // char *ind = test.create_autoindex((char *)"/Users/atomatoe/Desktop/webserv"); // создание ошибки // Утечка! Нужно чистить ind ответ после того как отправили ошибку клиенту!!!!!!
                    // char *err = test.create_error((char *)"404", (char *)"Page not found");
                   // it->second.buff_write = str_join(it->second.buff_write, ind);
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
                        close(it->first); // как отправили все данные клиенту, отсоединяемся
                        free(it->second.buff_write);
                        it->second.buff_write = NULL;
                    }
                }
            }
        }
    }
}
void Server::set_socket(char *ip, int port)
{
    address_len = (sizeof(socket_addr));
    socket_addr.sin_family = AF_INET;
    socket_addr.sin_addr.s_addr = inet_addr(ip); // содержит адрес (номер) узла сети.
    socket_addr.sin_port = htons(port); // 80 - port с конфига  что делает htons (80 >> 8 | 80 << 8)
}


int Server::get_server_fd() { return(server_fd); }
int Server::get_max_fd() { return(max_fd); }
fd_set *Server::get_fd_read() { return(&fd_read); }
fd_set *Server::get_fd_read_tmp() { return(&fd_read_tmp); }
fd_set *Server::get_fd_write() { return(&fd_write); }
fd_set *Server::get_fd_write_tmp() { return(&fd_write_tmp); }
std::map<int, t_client> &Server::get_list() { return(list); }
sockaddr_in *Server::get_socket_addr() { return(&socket_addr); }
socklen_t *Server::get_address_len() { return(&address_len); }
sockaddr_in *Server::get_out() { return(&out); }
void Server::set_max_fd(int count) { max_fd = count; }
void Server::set_fd_read(fd_set fd) { fd_read = fd; }
void Server::set_fd_read_tmp(fd_set fd) { fd_read_tmp = fd; }
void Server::set_fd_write(fd_set fd) { fd_write = fd; }
void Server::set_fd_write_tmp(fd_set fd) { fd_write_tmp = fd; }