/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 16:10:54 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/27 21:24:41 by atomatoe         ###   ########.fr       */
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
    struct timeval tv;
    this->_httpVersion = "HTTP/1.1 200 OK\r\n";
    this->_contentLength = "Content-length: ";
    gettimeofday(&tv, NULL);
    this->_timeOfResponse = get_time(tv.tv_sec);
    this->_versionOfWebServer = "Server: Webserv/1.0 (MacOS)";
    this->_location_id = -1;
    _lenOfResponse = 0;
}

Response::~Response() { }

std::string Response::give_me_index(std::string index)
{
    size_t i = index.size();
    std::string tmp;
    while(index[i] != '/')
        i--;
    while(index[i])
        tmp.push_back(index[i++]);
    return(tmp);
}

void Response::check_file_or_dir(Request request, WebServer server)
{
    int r_open;
    int r_read;
    std::string directory = server.getLocations()[_location_id].getRoot() + request.getURI();
    struct stat sb;
    
    if ((stat(directory.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) || (request.getURI() == server.getLocations()[_location_id].getUrl()))
        r_open = open(server.getLocations()[_location_id].getIndex().c_str(), O_RDONLY);
    else
    {
        directory = server.getLocations()[_location_id].getRoot() + ft_substr(request.getURI(), strlen(server.getLocations()[_location_id].getUrl().c_str()), strlen(request.getURI()));
        if ((stat(directory.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) || (request.getURI() == server.getLocations()[_location_id].getUrl()))
            r_open = open((directory + give_me_index(server.getLocations()[_location_id].getIndex())).c_str(), O_RDONLY);
        else
            r_open = open(directory.c_str(), O_RDONLY);
    }
	if (r_open < 0)
    {
       // std::cout << "this test +++++++";
        putErrorToBody((char *)"404", (char *)"Not found", server);
    }
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

int Response::search_uri(WebServer server, char *uri)
{
    char *tmp = strdup(uri);
    char *buf;
    int i = strlen(tmp);

    while(i != 0)
    {
        i = strlen(tmp);
        for(size_t it = 0; it != server.getLocations().size(); it++) {
            if ((server.getLocations()[it].getUrl()) == tmp) // проверка на существование такого location по запросу
                return(it);
        }
        while(tmp[i] != '/' && i != 1)
            i--;
        buf = ft_substr(tmp, 0, i);
        free(tmp);
        tmp = buf;
    }
    return(-1);
}

void Response::methodPut(Request request, WebServer server, Page_html page)
{
    int r_open;
    int r_read;
	this->_location_id = search_uri(server, request.getURI());
    std::string directory = server.getLocations()[_location_id].getRoot() + request.getURI();
   // std::cout << "directory = " << directory << std::endl;
    struct stat sb;

	if (!(server.getLocations()[this->_location_id].getAllowMethods()).find(request.getMetod())->second)
		putErrorToBody((char *)"405", (char *)"Method Not Allowed", server);
    else
    {     
        if (stat((server.getLocations()[this->_location_id].getRoot() + directory).c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
            putErrorToBody((char *)"404", (char *)"Запрос PUT не может идти на папку !!!!!", server);
        else
        {
            // std::cout << "zdes dolzno bit sozdanye faila" << std::endl;
            std::string tmp = request.getReqBody().toPointer();
           // std::cout << "tmp = \n" << tmp << std::endl;
            tmp.resize(atoi(request.getContentLength()));
            std::string str = server.getLocations()[this->_location_id].getRoot() + give_me_index(request.getURI());
           // std::cout << "str = " << str << std::endl;
            std::ofstream filename(str, std::ios::app); // std::ios::app - для записи в конец файла
            if(!filename)
                putErrorToBody((char *)"007", (char *)"File is not created!", server);
            filename << tmp;
            filename.close();
            if (request.getReqBody().getDataSize() == 0) {
            	_httpVersion = "HTTP/1.1 204 No Content\r\n";
            }
        }
    }
}

void Response::methodPost(Request request, WebServer server, Page_html page) {
    char *tmp;

	this->_location_id = search_uri(server, request.getURI());
    std::string directory = server.getLocations()[_location_id].getRoot() + request.getURI();
    struct stat sb;
    
	if (!(server.getLocations()[this->_location_id].getAllowMethods()).find(request.getMetod())->second)
		putErrorToBody((char *)"405", (char *)"Method Not Allowed", server);
    else
    {
    	//std::cout << "SIZE: " << request.getReqBody().getDataSize() << std::endl;
    	if (request.getReqBody().getDataSize() > server.getLocations()[this->_location_id].getLimitBody()) {
    		putErrorToBody((char *)"413", (char *)"Payload Too Large", server);
    		return;
    	}
        if (stat((server.getLocations()[this->_location_id].getRoot() + directory).c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
            putErrorToBody((char *)"404", (char *)"Запрос POST не может идти на папку !!!!!", server);
        else {
            request.setPathToCgi(std::string("/Users/welease/webserv/testing_cgi/cgi-bin/cgi_tester"));
            toCGI(*this, request, server);
           // tmp = _bodyOfResponse.toPointer();
        }
    }
}

void Response::methodGetHead(Request request, WebServer server, Page_html page)
{
    std::string directory;
    struct stat sb;
	char *tmp;
	int r_open, r_read;
    
    if(server.getAutoIndex())
	{
		directory = server.getRootPath() + request.getURI();
		if (stat(directory.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) {
			tmp = (char *)page.create_autoindex(directory.c_str(), request.getURI());
			_bodyOfResponse.addData(tmp, ft_strlen(tmp));
		}
		else // this is no dir
		{
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
      //  std::cout << "location id = " << this->_location_id << std::endl;
		if (!(server.getLocations()[this->_location_id].getAllowMethods()).find(request.getMetod())->second)
			putErrorToBody((char *)"405", (char *)"Method Not Allowed", server);
        else
            check_file_or_dir(request, server);
    }
}

char* Response::give_me_response(Request request, WebServer server)
{
	Page_html page;

    // -----------------------------------------------------
   // std::cout << std::endl;
   // std::cout << "URI: " << request.getURI() << std::endl;
    // std::cout << "Method: " << request.getMetod() << std::endl;
    // std::cout << "Server root path: " << server.getRootPath() << std::endl;
    // -----------------------------------------------------
    // Вывод запроса:
    // std::cout << request.getReqString() << std::endl;
    // std::cout << request.getReqBody().toPointer() << std::endl;
    // -----------------------------------------------------
    std::cout << "in give me \n";
    _location_id = -1;
    if((strcmp(request.getMetod(), "GET") == 0) || (strcmp(request.getMetod(), "HEAD") == 0)) {
        methodGetHead(request, server, page);
        if(strcmp(request.getMetod(), "HEAD") == 0) {
            _versionOfWebServer = "Server: Webserv/1.0 (MacOS)\r\n";
            std::string tmp1 = _httpVersion + _timeOfResponse + "\r\n" + _contentLength + std::to_string(_bodyOfResponse.getDataSize()) + doubleCRLF;
            _lenOfResponse = tmp1.length();
            return const_cast<char *>(tmp1.c_str());
        }
    }
    else if(strcmp(request.getMetod(), "POST") == 0)
        methodPost(request, server, page);
    else if(strcmp(request.getMetod(), "PUT") == 0)
        methodPut(request, server, page);
    else if((strcmp(request.getMetod(), "GET") != 0) && (strcmp(request.getMetod(), "PUT") != 0)
        && (strcmp(request.getMetod(), "POST") != 0) && (strcmp(request.getMetod(), "HEAD") != 0))
        this->_httpVersion = "HTTP/1.1 400 Bad Request\r\n";
    // std::cout << "Ответ:\n" << edit_response() << std::endl;
    return(edit_response(&request));
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

	std::map<std::string, std::string> error_p = server.getErrorPage();
	std::map<std::string, std::string>::iterator it = error_p.begin();
	while(it != error_p.end()) {
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

char* Response::edit_response(Request *request) {
	//int fd = open("/Users/welease/webserv/file", O_RDWR);
	if (strcmp(request->getMetod(), "POST") == 0) {
		char *tmp1 = _bodyOfResponse.toPointer();
		char *t;
		if ((t = static_cast<char *>(memmem( tmp1, _bodyOfResponse.getDataSize(), "\r\n\r\n", 4)))) {
			Bytes body = _bodyOfResponse.cutData(t - tmp1 + 4);
			_bodyOfResponse = body;
		}
	}
	std::string tmp = _httpVersion + _timeOfResponse + "\r\n" +  _contentLength + std::to_string(_bodyOfResponse.getDataSize()) + "\r\n" +  _versionOfWebServer + doubleCRLF;
	_lenOfResponse = tmp.length() + _bodyOfResponse.getDataSize();
	//write(fd, ft_memjoin((char *)tmp.c_str(), _bodyOfResponse.toPointer(), tmp.length(), _bodyOfResponse.getDataSize()), tmp.length() + _bodyOfResponse.getDataSize());
	return (ft_memjoin((char *)tmp.c_str(), _bodyOfResponse.toPointer(), tmp.length(), _bodyOfResponse.getDataSize()));
}

size_t Response::getLenOfResponse() const { return _lenOfResponse; }


//HTTP/1.1 200 OK
//		Date: Tue, 02 Mar 2021 15:39:23 GMT
//		Content-length: 345
//Server: Webserv/1.0 (MacOS)
//
//Status: 200 OK
//		Content-Type: text/html; charset=utf-8