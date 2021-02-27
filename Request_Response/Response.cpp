/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 16:10:54 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/25 18:54:10 by atomatoe         ###   ########.fr       */
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
    this->_contentLength = "Content-length: ";
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

    if(stat(uri, &sb) == 0 && S_ISDIR(sb.st_mode))
        return (-1);
    else
    {
        while(uri[i] != '/')
            i--;
        char* tmp = (char *)malloc(sizeof(char) * (i + 2));
        while(t != i) {
            tmp[t] = uri[t];
            t++;
        }
        tmp[t++] = '/';
        tmp[t] = '\0';
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
    while(uri[i]) {
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

void Response:: methodGet(Request request, WebServer server) {
	std::string directory;
	Page_html autoindex;
	struct stat sb;
	char *tmp;
	int r_open;
	int r_read;

	if(server.getAutoIndex() == true)
	{
		directory = server.getRootPath() + request.getURI();
		if (stat(directory.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) {
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
	else {
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

char* Response::give_me_response(Request request, WebServer server)
{
	Page_html autoindex;
	char	*tmp;
    // -----------------------------------------------------
    // std::cout << std::endl;
    // std::cout << "URI: " << request.getURI() << std::endl;
    // std::cout << "Method: " << request.getMetod() << std::endl;
    // std::cout << "Server root path: " << server.getRootPath() << std::endl;
    // -----------------------------------------------------
    // Вывод запроса:
    std::cout << request.getReqString() << std::endl;
    std::cout << request.getReqBody().toPointer() << std::endl;
    // ---------------


    _location_id = -1;
	/*CGI calling*/
    if(strcmp(request.getMetod(), "POST") == 0)
    {
        this->_location_id = search_location(server, request.getURI());
        if (_location_id == -1) {
        	putErrorToBody((char *)"405", (char *)"Method Not Allowed", server);
			_httpVersion = "HTTP/1.1 405 Method Not Allowed\r\n";
			this->_versionOfWebServer = "Server: Webserv/1.0 (MacOS)\r\n";
			std::cout << "RESPONSE: " << edit_response() << ":END OF RESPONSE" << std::endl;
			return (edit_response());
        }
//        std::cout << "location id = " << this->_location_id  << std::endl;
//        if (!(server.getLocations()[this->_location_id].getAllowMethods()).find("POST")->second)
//        {
//            std::cout << "this method non true" << std::endl;
//            putErrorToBody((char *)"405", (char *)"Method Not Allowed", server);
//        }
//        else
//        {
			std::cout << "\nTEST 123\n" << this->_location_id  << std::endl;
        	request.setPathToCgi(std::string("/Users/welease/webserv/testing_cgi/cgi-bin/html.py"));
			toCGI(*this, request, server);
			//std::cout << "+++" << _bodyOfResponse.toPointer() << "+++" << _bodyOfResponse.getDataSize() << "+++" << std::endl;
			tmp = _bodyOfResponse.toPointer();
			_bodyOfResponse.addData("", 1);
			_httpVersion = "HTTP/1.1 405 Method Not Allowed\r\n";
			this->_versionOfWebServer = "Server: Webserv/1.0 (MacOS)\r\nContent-length: 893\r\n";
         //   std::cout << edit_response() << std::endl;
		//	putErrorToBody((char *)"201", (char *)"Created", server);
            return (edit_response());
//        }
    }
	/*end of CGI*/
    else if(strcmp(request.getMetod(), "GET") == 0) {
        methodGet(request, server);
    }
    else if(strcmp(request.getMetod(), "PUT") == 0)
    {
        this->_location_id = search_location(server, request.getURI());
        if(this->_location_id == -1)
            putErrorToBody((char *)"404", (char *)"Not found", server);
        else if (!(server.getLocations()[this->_location_id].getAllowMethods()).find("PUT")->second)
                putErrorToBody((char *)"405", (char *)"Method Not Allowed", server);
        else
        {
            // std::cout << "zdes dolzno bit sozdanye faila" << std::endl;
            std::string tmp = request.getReqBody().toPointer();
            tmp.resize(atoi(request.getContentLength()));
            std::cout << "tmp = \n" << tmp << std::endl;
            std::string str = server.getLocations()[this->_location_id].getRoot() + delete_locations(request.getURI());
            std::ofstream filename(str, std::ios::app); // std::ios::app - для записи в конец файла
            if(!filename)
                putErrorToBody((char *)"007", (char *)"File is not created!", server);
            filename << tmp;
            filename.close();
        }
    }
    else if(strcmp(request.getMetod(), "HEAD") == 0)
    {
    	methodGet(request, server);
        //if(search_uri(server, request.getURI()) == -1)
        //    this->_httpVersion = "HTTP/1.1 404 Not Found\n";
        // else if (!(server.getLocations()[this->_location_id].getAllowMethods()).find("HEAD")->second)
        //         putErrorToBody((char *)"405", (char *)"Method Not Allowed", server);
        _versionOfWebServer = "Server: Webserv/1.0 (MacOS)\r\n";
		std::string tmp1 = _httpVersion + _timeOfResponse + "\r\n" + _versionOfWebServer + _contentLength + std::to_string(_bodyOfResponse.getDataSize()) + doubleCRLF;
		std::cout << "++++++" << tmp1 << "++++++++" << std::endl;
		_lenOfResponse = tmp1.length();
		return const_cast<char *>(tmp1.c_str());
    }
    else if((strcmp(request.getMetod(), "GET") != 0) && (strcmp(request.getMetod(), "PUT") != 0) && (strcmp(request.getMetod(), "POST") != 0)
    && (strcmp(request.getMetod(), "GET") != 0) && (strcmp(request.getMetod(), "HEAD") != 0))
        this->_httpVersion = "HTTP/1.1 400 Bad Request\n";



    //std::cout << edit_response() << std::endl;
    return(edit_response());
}

char* Response::edit_response() {
	std::string tmp = _httpVersion + _timeOfResponse + "\r\n" + _versionOfWebServer + _contentLength + std::to_string(_bodyOfResponse.getDataSize()) + doubleCRLF;
	_lenOfResponse = tmp.length() + _bodyOfResponse.getDataSize();
	return (ft_memjoin((char *)tmp.c_str(), _bodyOfResponse.toPointer(), tmp.length(), _bodyOfResponse.getDataSize()));
}

size_t Response::getLenOfResponse() const {
	return _lenOfResponse;
}