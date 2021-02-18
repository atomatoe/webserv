/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 16:10:54 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/18 17:19:50 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response()
{
    this->_h1 = "HTTP/1.1 200 OK\n";
    struct timeval tv;
    gettimeofday(&tv, NULL);
    this->_h2 = get_time(tv.tv_sec);
    this->_h3 = "Server: Webserv/1.0 (MacOS)\n\n";
    this->_response = NULL;
    this->_location_id = 0;
}

Response::~Response() { }

int Response::search_uri(WebServer server, char *uri)
{
    size_t it = 0;

    while(it != server.get_location().size())
    {
        if ((server.get_location()[it].get_uri()) == uri) // проверка на существование такого location по запросу
        {
            this->_location_id = it;
            return(0);
        }
        it++;
    }
    return(-1);
}

int Response::check_uri(WebServer server, char *uri)
{
    struct stat sb;
    size_t i = strlen(uri);
    size_t t = 0;

    if(stat(uri, &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        std::cout << "testers" << std::endl;
        return (-1);
    }
    else
    {
        while(uri[i] != '/')
            i--;
        char* tmp = (char *)malloc(sizeof(char) * (i + 2));
        while(t != i)
        {
            tmp[t] = uri[t];
            t++;
        }
        tmp[t] = '/';
        t++;
        tmp[t] = '\0';
        // std::cout << "uri 2= " << tmp << std::endl;
        return(search_uri(server, tmp));
    }
    return(0);

}

char* Response::give_me_response(Request request, WebServer server)
{
    std::string directory;
    Page_html errors; // для генерации ошибок         

    std::cout << "URI: " << request.getURI() << std::endl;
    std::cout << "Method: " << request.getMetod() << std::endl;

    if(request.getMetod()[0] == 'G' && request.getMetod()[1] == 'E' && request.getMetod()[2] == 'T')
    {
        if(search_uri(server, request.getURI()) == -1 && check_uri(server, request.getURI()) == -1)
        {
            this->_h1 = "HTTP/1.1 404 Not Found\n"; // std::cout << "такого location не существует" << std::endl;
            std::cout << server.get_error_page().find((char *)"404")->first << std::endl;
            if(server.get_error_page().find((char *)"404")->first == server.get_error_page().end()->first) // std::cout << "error page отсутствует" << std::endl;
                this->_body = errors.create_error((char *)"404", (char *)"Not Found");
            else // std::cout << "error page присутствует" << std::endl;
            {
                int fd_tmp = open(server.get_error_page().find((char *)"404")->second.c_str(), O_RDONLY);
                if(fd_tmp < 0)
                    this->_body = errors.create_error((char *)"01", (char *)"The file cannot be open, bitch");
                else
                {
                    char* temp = (char *)malloc(sizeof(char) * 4097);
                    int i = read(fd_tmp, temp, 5555);
                    if(i < 0)
                        this->_body = errors.create_error((char *)"02", (char *)"The file cannot be read, bitch");
                    else
                        this->_body = temp;
                    close(fd_tmp);
                }
            }
        }
        else
        {
            if ((server.get_location()[this->_location_id].get_method()).find("GET")->second == false)
            {
                this->_h1 = "HTTP/1.1 405 Method Not Allowed\n"; // std::cout << "такой метод запрещен!" << std::endl;
                if(server.get_error_page().find((char *)"405")->first == server.get_error_page().end()->first) // std::cout << "error page отсутствует" << std::endl;
                    this->_body = errors.create_error((char *)"405", (char *)"Method Not Allowed");
                else // std::cout << "error page присутствует" << std::endl;
                {
                    int fd_tmp = open(server.get_error_page().find((char *)"405")->second.c_str(), O_RDONLY);
                    if(fd_tmp < 0)
                        this->_body = errors.create_error((char *)"03", (char *)"The file cannot be open, bitch");
                    else
                    {
                        char* temp = (char *)malloc(sizeof(char) * 4097);
                        int i = read(fd_tmp, temp, 5555);
                        if(i < 0)
                            this->_body = errors.create_error((char *)"04", (char *)"The file cannot be read, bitch");
                        else
                            this->_body = temp;
                        close(fd_tmp);
                    }
                }
            }
            else
            {
                if(server.get_location()[this->_location_id].get_index() == ""  && server.get_location()[this->_location_id].get_autoindex() == false)
                {
                    this->_h1 = "HTTP/1.1 204 No Content\n";
                    this->_body = errors.create_error((char *)"204", (char *)"No Content");
                }
                else
                {
                    struct stat sb;
                    if (stat(request.getURI(), &sb) == 0 && S_ISDIR(sb.st_mode))
                    {
                        std::cout << "this directory" << std::endl;
                        if(server.get_location()[this->_location_id].get_autoindex() == true)
                            this->_body = errors.create_autoindex(server.get_location()[this->_location_id].get_root() + request.getURI());
                        else
                        {
                            directory = server.get_location()[this->_location_id].get_root() + '/' + server.get_location()[this->_location_id].get_index();
                            std::cout << "dir/file = " << directory << std::endl;
                            int fd = open(directory.c_str(), O_RDONLY);
                            if(fd < 0)
                                this->_body = errors.create_error((char *)"05", (char *)"The file cannot be open, bitch");
                            else
                            {
                                char* hello = (char *)malloc(sizeof(char) * 4097);
                                int i = read(fd, hello, 5555);
                                if(i < 0)
                                    this->_body = errors.create_error((char *)"06", (char *)"The file cannot be read, bitch");
                                else
                                    this->_body = hello;
                                close(fd);
                            }
                        }
                    }
                    else
                    {
                        std::cout << "this no directory" << std::endl;
                        directory = server.get_location()[this->_location_id].get_root() + request.getURI();
                        if (stat(directory.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
                        {
                            this->_body = errors.create_autoindex(directory.c_str());
                        }
                        else
                        {
                            std::cout << directory << std::endl;
                            int fd = open(directory.c_str(), O_RDONLY);
                            if(fd < 0)
                                this->_body = errors.create_error((char *)"07", (char *)"The file cannot be open, bitch");
                            else
                            {
                                char* hello = (char *)malloc(sizeof(char) * 4097);
                                int i = read(fd, hello, 5555);
                                if(i < 0)
                                    this->_body = errors.create_error((char *)"08", (char *)"The file cannot be read, bitch");
                                else
                                    this->_body = hello;
                                close(fd);
                            }
                        }
                    }
                }
            }
        }
    }
    else if(request.getMetod()[0] == 'P' && request.getMetod()[1] == 'U' && request.getMetod()[2] == 'T')
    {
        if(search_uri(server, request.getURI()) == -1 && check_uri(server, request.getURI()) == -1)
        {
            this->_h1 = "HTTP/1.1 404 Not Found\n"; // std::cout << "такого location не существует" << std::endl;
            if(server.get_error_page().find((char *)"404")->first == server.get_error_page().end()->first) // std::cout << "error page отсутствует" << std::endl;
                this->_body = errors.create_error((char *)"404", (char *)"Not Found");
            else // std::cout << "error page присутствует" << std::endl;
            {
                int fd_tmp = open(server.get_error_page().find((char *)"404")->second.c_str(), O_RDONLY);
                if(fd_tmp < 0)
                    this->_body = errors.create_error((char *)"09", (char *)"The file cannot be open, bitch");
                else
                {
                    char* temp = (char *)malloc(sizeof(char) * 4097);
                    int i = read(fd_tmp, temp, 5555);
                    if(i < 0)
                        this->_body = errors.create_error((char *)"10", (char *)"The file cannot be read, bitch");
                    else
                        this->_body = temp;
                    close(fd_tmp);
                }
            }
        }
        else
        {
            if ((server.get_location()[this->_location_id].get_method()).find("PUT")->second == false)
            {
                this->_h1 = "HTTP/1.1 405 Method Not Allowed\n"; // std::cout << "такой метод запрещен!" << std::endl;
                if(server.get_error_page().find((char *)"405")->first == server.get_error_page().end()->first) // std::cout << "error page отсутствует" << std::endl;
                    this->_body = errors.create_error((char *)"405", (char *)"Method Not Allowed");
                else // std::cout << "error page присутствует" << std::endl;
                {
                    int fd_tmp = open(server.get_error_page().find((char *)"405")->second.c_str(), O_RDONLY);
                    if(fd_tmp < 0)
                        this->_body = errors.create_error((char *)"11", (char *)"The file cannot be open, bitch");
                    else
                    {
                        char* temp = (char *)malloc(sizeof(char) * 4097);
                        int i = read(fd_tmp, temp, 5555);
                        if(i < 0)
                            this->_body = errors.create_error((char *)"12", (char *)"The file cannot be read, bitch");
                        else
                            this->_body = temp;
                        close(fd_tmp);
                    }
                }
            }
            else
            {
                // std::cout << "content lenght = " << request.getContentLength() << std::endl;
                // int t = atoi(request.getContentLength());
                int t = 60;
                std::string tmp = request.getReqString();
                size_t it = tmp.find("\r\n\r\n");
                it += 4; // пропускаем /r/n/r/n, чтобы быть на первой букве body
                std::string buff;
                for(size_t i = 0; i != t; i++)
                {
                    buff += tmp[it++];
                }
                std::string str = server.get_location()[this->_location_id].get_root() + request.getURI();
                std::ofstream filename(str, std::ios::app); // std::ios::app - для записи в конец файла
                if(!filename)
                {
                    std::cout << "open file error!" << std::endl;
                }
                filename << buff;
                filename.close();
            }
        }
    }
    else if(request.getMetod()[0] == 'H' && request.getMetod()[1] == 'E' && request.getMetod()[2] == 'A' && request.getMetod()[3] == 'D')
    {
        if(search_uri(server, request.getURI()) == -1 && check_uri(server, request.getURI()) == -1)
            this->_h1 = "HTTP/1.1 404 Not Found\n";
        if ((server.get_location()[this->_location_id].get_method()).find("PUT")->second == false)
            this->_h1 = "HTTP/1.1 405 Method Not Allowed\n";
    }
    else if((strcmp(request.getMetod(), "GET") != 0) && (strcmp(request.getMetod(), "PUT") != 0) && (strcmp(request.getMetod(), "POST") != 0) && (strcmp(request.getMetod(), "GET") != 0) && (strcmp(request.getMetod(), "HEAD") != 0))
        this->_h1 = "HTTP/1.1 400 Bad Request\n";
    return(edit_response());
}

char* Response::edit_response()
{
    size_t it = 0, ik = 0;
    this->_response = (char *)malloc(10001); // разобраться, не очищается response (попробуй выстави malloc 500 и обнови страницу раз 5)
    while(_h1[it])
    {
        this->_response[it] = _h1[it];
        it++;
    }
    while(_h2[ik])
    {
        this->_response[it] = _h2[ik];
        it++; ik++;
    }
    ik = 0;
    while(_h3[ik])
    {
        this->_response[it] = _h3[ik];
        it++; ik++;
    }
    ik = 0;
    while(_body[ik])
    {
        this->_response[it] = _body[ik];
        it++; ik++;
    }
    this->_response[it] = '\0';
    return(this->_response);
}