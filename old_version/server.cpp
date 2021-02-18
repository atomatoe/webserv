/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 15:09:20 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/12 20:43:18 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/page_html.hpp"
#include "server.hpp"

Server::Server()
{
    max_fd = 0;
    yes = 1;
    status = strdup("HTTP/1.1 200 OK\n");
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
                    // std::cout << "запрос: " << it->second.buff_read << std::endl;
					Request request(it->second.buff_read);
                    // std::cout << "This test: " << request.getReqString() << std::endl;
                    http_metods_put_post(request);
                    it->second.buff_write = str_join(it->second.buff_write, give_me_headers());
					// it->second.buff_write = str_join(it->second.buff_write, (char *)"HTTP/1.1 200 OK\nDate: Mon, 27 Jul 2009 12:28:53 GMT\nServer: Apache/2.2.14 (Win32)\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\nContent-Type: text/html\nConnection: Closed\n\n");
					//toCGI(request, &(it->second.buff_write)); //todo error handler
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
                    // int fd = open((char *)"index.html", O_RDONLY);
                    // char* hello = (char *)malloc(sizeof(char) * 4097);
                    // read(fd, hello, 5555);
                    // std::cout << hello << std::endl;
                    // вот здесь прикручиваем в первой части ответа html страницу, если она нужна
                    // char *ind = test.create_autoindex((char *)"/Users/atomatoe/Desktop/webserv"); // создание ошибки // Утечка! Нужно чистить ind ответ после того как отправили ошибку клиенту!!!!!!
                    // char *err = test.create_error((char *)"404", (char *)"Page not found");
                    // it->second.buff_write = http_metods_get_head(request);
                    Request request(it->second.buff_read);
                    it->second.buff_write = str_join(it->second.buff_write, http_metods_get_head(request));
                    // std::cout << "TEST: " << it->second.buff_write << std::endl;
                    int ret = send(it->first, it->second.buff_write, strlen(it->second.buff_write), 0);
                    free(status); status = NULL;
                    status = strdup("HTTP/1.1 200 OK\n");
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
void Server::http_metods_put_post(Request request)
{
    // std::cout << "Test:" << request.getReqString() << std::endl;
    // std::cout << "Method: " << request.getMetod() << std::endl;

    // PUT  ----------------------------
    if(request.getMetod()[0] == 'P' && request.getMetod()[1] == 'U' && request.getMetod()[2] == 'T')
    {
        // int t = atoi(request.getContentLength());
        int t = 60;
        std::string tmp = request.getReqString();
        size_t it = tmp.find("\r\n\r\n");
        it += 4; // пропускаем /r/n/r/n, чтобы быть на первой букве body
        std::string buff;
        int fd;
        for(size_t i = 0; i != t; i++)
        {
            buff += tmp[it++];
        }
        std::ifstream in(request.getURI());
        if(!in.is_open())
            std::cout << "файл не существует!!" << std::endl;
        else
        {
            std::ofstream filename(request.getURI(), std::ios::app); // std::ios::app - для записи в конец файла
            if(!filename)
            {
                std::cout << "open file error!" << std::endl;
                exit(1);
            }
            filename << buff;
            filename.close();
        }
        in.close();
    }
    // POST  ----------------------------
    else if(request.getMetod()[0] == 'P' && request.getMetod()[1] == 'O' && request.getMetod()[2] == 'S' && request.getMetod()[3] == 'T')
    {
        // int t = atoi(request.getContentLength());
        int t = 60;
        std::string tmp = request.getReqString();
        size_t it = tmp.find("\r\n\r\n");
        it += 4; // пропускаем /r/n/r/n, чтобы быть на первой букве body
        std::string buff;
        int fd;
        for(size_t i = 0; i != t; i++)
        {
            buff += tmp[it++];
        }
        std::ofstream filename(request.getURI(), std::ios::app); // std::ios::app - для записи в конец файла
        if(!filename)
        {
            std::cout << "open file error!" << std::endl;
            exit(1);
        }
        filename << buff;
        filename.close();
    }
    else if((strcmp(request.getMetod(), "GET") != 0) && (strcmp(request.getMetod(), "PUT") != 0) && (strcmp(request.getMetod(), "POST") != 0) && (strcmp(request.getMetod(), "GET") != 0) && (strcmp(request.getMetod(), "HEAD") != 0))
    {
        std::cout << "Bad Request" << std::endl;
        free(status); status = NULL;
        status = strdup("HTTP/1.1 400 Bad Request\n");
    }
    if(request.getMetod()[0] == 'G' && request.getMetod()[1] == 'E' && request.getMetod()[2] == 'T')
    {
        int fds = open(request.getURI(), O_RDONLY);
        char* hello = (char *)malloc(sizeof(char) * 4097); // сделать выделение памяти!
        if(fds == -1)
        {
            free(status); status = NULL;
            status = strdup("HTTP/1.1 404 Not Found\n");
        }
        close(fds);
    }
    if((request.getMetod()[0] == 'G' && request.getMetod()[1] == 'E' && request.getMetod()[2] == 'T') && (strcmp(request.getURI(), "") == 0)) // стартовая страница
    {
        free(status); status = NULL;
        status = strdup("HTTP/1.1 200 OK\n");
    }
}

char* Server::http_metods_get_head(Request request)
{
    if(request.getMetod()[0] == 'G' && request.getMetod()[1] == 'E' && request.getMetod()[2] == 'T')
    {
        // std::cout << "URI = " << request.getURI() << std::endl;
        int fd = open(request.getURI(), O_RDONLY);
        if(strcmp(request.getURI(), "") == 0)
            fd = open("index.html", O_RDONLY);
        if(fd == -1)
        {
            std::cout << "(error 44) open error http method = " << request.getURI() << std::endl;
        }
        char* hello = (char *)malloc(sizeof(char) * 4097); // сделать выделение памяти!
        if((read(fd, hello, 5555)) == -1) // Исправить 5555, может быть gnl?
        {
            std::cout << "(error 52) read error http method = " << request.getURI() << std::endl;
        }
        close(fd);
        return(hello);
    }
    // так как у нас уже сформированы хедеры, то нам ничего объединять не нужно (не надо описывать метод HEAD)
    return (NULL);
}

char* Server::give_me_headers()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    char *h_2 = get_time(tv.tv_sec);
    char h_3[30] = "Server: Webserv/1.0 (MacOS)\n\n";
    char *tmp = (char *)malloc(300);
    size_t i = 0;
    size_t t = 0;
    while(this->status[i] != '\0')
    { tmp[i] = this->status[i]; i++; }
    while(h_2[t] != '\0')
    { tmp[i] = h_2[t++]; i++; }
    t = 0;
    while(h_3[t] != '\0')
    { tmp[i] = h_3[t++]; i++; }
    tmp[i] = '\0';
    // std::cout << tmp << std::endl;
    return(tmp);
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