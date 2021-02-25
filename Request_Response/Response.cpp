/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 16:10:54 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/24 18:52:26 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

/*
GET ─получить данные
PUT ─ создать данные
POST ─ редактировать данные
DELETE ─ удалить данные
*/

Response::Response()
{
    this->_httpVersion = "HTTP/1.1 200 OK\r\n";
    struct timeval tv;
    gettimeofday(&tv, NULL);
    this->_timeOfResponse = get_time(tv.tv_sec);
    this->_versionOfWebServer = "Server: Webserv/1.0 (MacOS)\r\n\r\n";
    this->_location_id = -1;
    _lenOfResponse = 0;
}

Response::~Response() { }

int Response::search_uri(WebServer server, char *uri)
{
    for(size_t it = 0; it != server.getLocations().size(); it++) {
        if ((server.getLocations()[it].getUrl()) == uri) // проверка на существование такого location по запросу
            return(it);
    }
    return(-1);
}

int Response::search_location(WebServer server, char *uri)
{
    struct stat sb;
    size_t i = strlen(uri);
    size_t t = 0;
    int ret;

 //   std::cout << "uri 51 = " << uri << std::endl;

    if(stat(uri, &sb) == 0 && S_ISDIR(sb.st_mode)) {
 //       std::cout << "testers" << std::endl;
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
        ret = search_uri(server, tmp);
        free(tmp);
        return ret;
    }
    return(0);
}

char* delete_locations(char *uri)
{
    size_t it = 0;
    size_t i = strlen(uri);
    char* temp = (char *)malloc(sizeof(char) * i);
    
    while(uri[i] != '/')
        i--;
    while(uri[i])
    {
        temp[it] = uri[i];
        it++;
        i++;
    }
    temp[it] = '\0';
    return(temp);
}

void Response::putErrorToBody(char *error, char *type, WebServer server)
{
	char 		*tmp;
    int         fd;
	Page_html errors; // для генерации ошибок

    this->_httpVersion = "HTTP/1.1 ";
    this->_httpVersion += error;
    this->_httpVersion += " ";
    this->_httpVersion += type;
    this->_httpVersion += "\n";
    std::map<std::string, std::string>::iterator it = server.getErrorPage().begin();
    while(it != server.getErrorPage().end())
    {
     //   std::cout << it->first << std::endl;
        if(it->first == error)
        {
            fd = open(server.getErrorPage()[error].c_str(), O_RDONLY);
            if(fd < 0) // file not open
				putErrorToBody((char *)"404", (char *)"Not found", server);
            else
            {
                char* temp = (char *)malloc(sizeof(char) * 4097);
                int i = read(fd, temp, 5555);
                if(i < 0)
                    putErrorToBody((char *)"000", (char *)"The file cannot be read, bitch", server);
                else
                    _bodyOfResponse.addData(temp, ft_strlen(temp));
                free(temp);
                close(fd);
            }
            return ;
        }
        it++;
    }
	tmp = (char *)errors.create_error(error, type);
	_bodyOfResponse.addData(tmp, ft_strlen(tmp));
}

char* Response::give_me_response(Request request, WebServer server)
{
	Page_html autoindex;
    struct stat sb;
    std::string directory;
    char *tmp;
    int r_open;
    int r_read;

//    std::cout <<

    // -----------------------------------------------------
   // std::cout << std::endl;
    bool _autoindex = true;
   // std::cout << "URI: " << request.getURI() << std::endl;
   // std::cout << "Method: " << request.getMetod() << std::endl;
   // std::cout << "Server root path: " << server.getRootPath() << std::endl;
    // -----------------------------------------------------

    _location_id = -1;
	/*CGI calling*/
    if(strcmp(request.getMetod(), "POST") == 0)
    {
        this->_location_id = search_location(server, request.getURI());
//        std::cout << "location id = " << this->_location_id  << std::endl;
        if(this->_location_id == -1)
        {
            putErrorToBody((char *)"411", (char *)"Length Required", server);
        }
        else if (!(server.getLocations()[this->_location_id].getAllowMethods()).find("POST")->second)
                putErrorToBody((char *)"405", (char *)"Method Not Allowed", server);
        else
        {
			std::cout << "TEST 23\n";
        	request.setPathToCgi(std::string("/Users/welease/webserv/testing_cgi/cgi-bin/html.py"));
			toCGI(*this, request, server);
			//std::cout << "+++" << _bodyOfResponse.toPointer() << "+++" << _bodyOfResponse.getDataSize() << "+++" << std::endl;
			tmp = _bodyOfResponse.toPointer();
			_bodyOfResponse.addData("", 1);
			this->_versionOfWebServer = "Server: Webserv/1.0 (MacOS)\r\nContent-Length: 142";
//            std::cout << edit_response() << std::endl;
            return (edit_response());
        }
    }
	/*end of CGI*/
    else if(strcmp(request.getMetod(), "GET") == 0)
    {
        if(_autoindex == true)
        {
            directory = server.getRootPath() + request.getURI();
//            std::cout << "directory: " << directory << std::endl;
            if (stat(directory.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
            {
                // std::cout << "is dir" << std::endl;
                tmp = (char *)autoindex.create_autoindex(directory.c_str(), request.getURI());
			    _bodyOfResponse.addData(tmp, ft_strlen(tmp));
            }
            else
            {
                // std::cout << "no dir" << std::endl;
                r_open = open(directory.c_str(), O_RDONLY);
                if (r_open < 0)
                    putErrorToBody((char *)"404", (char *)"Not found", server);
                else
                {
                    char* temp = (char *)malloc(sizeof(char) * (20000000));
                    r_read = read(r_open, temp, 20000000);
                    if(r_read < 0)
                        putErrorToBody((char *)"000", (char *)"The file cannot be read, bitch", server);
                    else
                        _bodyOfResponse.addData(temp, r_read);
                    free(temp);
                    close(r_open);
                }
            }
        }
        else
        {
            this->_location_id = search_uri(server, request.getURI());
            // std::cout << "location id = " << this->_location_id  << std::endl;
            if(this->_location_id == -1)
                putErrorToBody((char *)"404", (char *)"Not found", server);
            else if (!(server.getLocations()[this->_location_id].getAllowMethods()).find("GET")->second)
                putErrorToBody((char *)"405", (char *)"Method Not Allowed", server);
            else
            {
                directory = server.getLocations()[this->_location_id].getIndex();
                // std::cout << "directory: " << directory << std::endl;
                r_open = open(directory.c_str(), O_RDONLY);
                if (r_open < 0)
                    putErrorToBody((char *)"404", (char *)"Not found", server);
                else
                {
                    char* temp = (char *)malloc(sizeof(char) * (20000000));
                    r_read = read(r_open, temp, 20000000);
                    if(r_read < 0)
                        putErrorToBody((char *)"000", (char *)"The file cannot be read, bitch", server);
                    else
                        _bodyOfResponse.addData(temp, r_read);
                    free(temp);
                    close(r_open);
                }
            }
        }
    }
    else if(strcmp(request.getMetod(), "PUT") == 0)
    {
        this->_location_id = search_location(server, request.getURI());
//        std::cout << "location id = " << this->_location_id  << std::endl;
        if(this->_location_id == -1)
            putErrorToBody((char *)"404", (char *)"Not found", server);
        else if (!(server.getLocations()[this->_location_id].getAllowMethods()).find("PUT")->second)
                putErrorToBody((char *)"405", (char *)"Method Not Allowed", server);
        else
        {
//            std::cout << "zdes dolzno bit sozdanye faila" << std::endl;
            // // std::cout << "content lenght = " << request.getContentLength() << std::endl;
            // // int t = atoi(request.getContentLength());
            // int t = 60;
            // std::string tmp = request.getReqString();
            // size_t it = tmp.find("\r\n\r\n");
            // it += 4; // пропускаем /r/n/r/n, чтобы быть на первой букве body
            // std::string buff;
            // std::cout << "it = " << it << std::endl;
            // for(size_t i = 0; i != t; i++)
            //     buff += tmp[it++];
            // std::string str = server.getLocations()[this->_location_id].getRoot() + delete_locations(request.getURI());
            // std::cout << "str = " << str << std::endl;
            // std::ofstream filename(str, std::ios::app); // std::ios::app - для записи в конец файла
            // if(!filename)
            //     putErrorToBody((char *)"007", (char *)"File is not created!", server);
            // filename << buff;
            // std::cout << "buff = " << buff << std::endl;
            // filename.close();
        }
    }
    else if(strcmp(request.getMetod(), "HEAD") == 0)
    {
        if(search_uri(server, request.getURI()) == -1)
            this->_httpVersion = "HTTP/1.1 404 Not Found\n";
        // else if (!(server.getLocations()[this->_location_id].getAllowMethods()).find("HEAD")->second)
        //         putErrorToBody((char *)"405", (char *)"Method Not Allowed", server);
    }
    else if((strcmp(request.getMetod(), "GET") != 0) && (strcmp(request.getMetod(), "PUT") != 0) && (strcmp(request.getMetod(), "POST") != 0)
    && (strcmp(request.getMetod(), "GET") != 0) && (strcmp(request.getMetod(), "HEAD") != 0))
        this->_httpVersion = "HTTP/1.1 400 Bad Request\n";
    // std::cout << edit_response() << std::endl;
    return(edit_response());
}

char* Response::edit_response() {
	size_t size;
	size = _httpVersion.length() + _timeOfResponse.length() + _versionOfWebServer.length() + 2;
	std::string tmp = _httpVersion + _timeOfResponse + "\r\n" + _versionOfWebServer;
	//_bodyOfResponse.addData((char *)"", 1);
	_lenOfResponse = size + _bodyOfResponse.getDataSize();
	//std::cout << ft_memjoin((char *)tmp.c_str(), _bodyOfResponse.toPointer(), size, _bodyOfResponse.getDataSize()) << std::endl;
	return (ft_memjoin((char *)tmp.c_str(), _bodyOfResponse.toPointer(), size, _bodyOfResponse.getDataSize()));
}

size_t Response::getLenOfResponse() const {
	return _lenOfResponse;
}