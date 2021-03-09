/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 16:10:54 by atomatoe          #+#    #+#             */
/*   Updated: 2021/03/08 20:43:34 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "../../AuthClients/base64.h"

Response::Response() {
	char *			tmp;
    struct timeval 	tv;

    this->_httpVersion = "HTTP/1.1 200 OK\r\n";
    this->_contentLength = "Content-length: ";
    gettimeofday(&tv, NULL);
    tmp = get_time(tv.tv_sec);
    _timeOfResponse = tmp;
    free(tmp);
    this->_versionOfWebServer = "Server: Webserv/1.0 (MacOS)";
    this->_location_id = -1;
    _lenOfResponse = 0;
}

Response::~Response() {}

std::string Response::indexSearching(std::string index) {
    size_t i = index.size();
    std::string tmp;
    while(index[i] != '/')
        i--;
    while(index[i])
        tmp.push_back(index[i++]);
    return(tmp);
}

void Response::checkFileOrDir(Request & request, WebServer & server) {
    int r_open;
    int r_read;
    std::string directory = server.getLocations()[_location_id].getRoot() + request.getURI();
    struct stat sb;

    if ((stat(directory.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) || (request.getURI() == server.getLocations()[_location_id].getUrl()))
        r_open = open(server.getLocations()[_location_id].getIndex().c_str(), O_RDONLY);
    else {
        directory = server.getLocations()[_location_id].getRoot() +  "/" + request.getURI().substr(server.getLocations()[_location_id].getUrl().length(), request.getURI().length());
        if ((stat(directory.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) || (request.getURI() == server.getLocations()[_location_id].getUrl()))
            r_open = open((directory + indexSearching(server.getLocations()[_location_id].getIndex())).c_str(), O_RDONLY);
        else
            r_open = open(directory.c_str(), O_RDONLY);
    }
	if (r_open < 0)
        putErrorToBody((char *)"404", (char *)"Not found", server);
	else {
		char* temp = (char *)malloc(sizeof(char) * (bufSize));
		while ((r_read = read(r_open, temp, bufSize)) > 0)
			_bodyOfResponse.addData(temp, r_read);
		if	(r_read < 0)
			putErrorToBody((char *)"000", (char *)"The file cannot be read, bitch", server);
	    free(temp);
	    close(r_open);
	}  
}

int Response::uriSearching(WebServer & server, char *uri) {
    char *tmp = strdup(uri);
    char *buf;
    int i = strlen(tmp);

    while(i != 0) {
        i = strlen(tmp);
        for (size_t it = 0; it != server.getLocations().size(); it++) {
            if ((server.getLocations()[it].getUrl()) == tmp) {
				free(tmp);
            	return (it);
			}
        }
        while (tmp[i] != '/' && i != 1)
            i--;
        buf = ft_substr(tmp, 0, i);
        free(tmp);
        tmp = buf;
    }
    free(tmp);
    return(-1);
}

void Response::methodPut(Request & request, WebServer & server) {
    char *t;
	this->_location_id = uriSearching(server, (char *) request.getURI().c_str());
    std::string directory = server.getLocations()[_location_id].getRoot() + request.getURI();
    struct stat sb;

	if (!(server.getLocations()[this->_location_id].getAllowMethods()).find(request.getMetod())->second)
		putErrorToBody((char *)"405", (char *)"Method Not Allowed", server);
    else if (check_auth(request, server.getLocations()[this->_location_id]) == -1)
        putErrorToBody((char *)"401", (char *)"Unauthorized", server);
    else {
        if (stat((server.getLocations()[this->_location_id].getRoot() + directory).c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
            putErrorToBody((char *)"404", (char *)"Запрос PUT не может идти на папку !!!!!", server);
        else {
            t = request.getReqBody().toPointer();
            std::string tmp = t;
            free(t);
            std::string str = server.getLocations()[this->_location_id].getRoot() + indexSearching(request.getURI());
            std::ofstream filename(str, std::ios::app);
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

void Response::methodPost(Request & request, WebServer & server) {
	this->_location_id = uriSearching(server, (char *) request.getURI().c_str());
    std::string directory = server.getLocations()[_location_id].getRoot() + request.getURI();
    struct stat sb;
    
	if (!(server.getLocations()[this->_location_id].getAllowMethods()).find(request.getMetod())->second)
		putErrorToBody((char *)"405", (char *)"Method Not Allowed", server);
    else if (check_auth(request, server.getLocations()[this->_location_id]) == -1)
        putErrorToBody((char *)"401", (char *)"Unauthorized", server);
    else {
    	if (request.getReqBody().getDataSize() > (size_t)server.getLocations()[this->_location_id].getLimitBody()) {
    		putErrorToBody((char *)"413", (char *)"Payload Too Large", server);
    		return;
    	}
        if (stat((server.getLocations()[this->_location_id].getRoot() + directory).c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
            putErrorToBody((char *)"404", (char *)"Post request can't go to the folder", server);
        else {
            // std::cout << server.getLocations()[_location_id].getCgiPath() << std::endl;
            request.setPathToCgi(std::string("/Users/qtamaril/Desktop/qtamaril/webserv/TestingCGI/cgi-bin/cgi_tester"));
            try {
				toCGI(*this, request, server);
			}
            catch (std::exception & ex) {
            	putErrorToBody((char *)"500", (char *)"Bad GateAway", server);
            }
        }
    }
}

void Response::methodGetHead(Request & request, WebServer & server, Page_html & page) {
    std::string directory;
    struct stat sb;
	char *tmp;
	int r_open, r_read;
    
    if(server.getAutoIndex()) {
		directory = server.getRootPath() + request.getURI();
		if (stat(directory.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) {
			tmp = (char *)page.create_autoindex(directory.c_str(), (char *)request.getURI().c_str());
			_bodyOfResponse.addData(tmp, strlen(tmp));
		}
		else {
			r_open = open(directory.c_str(), O_RDONLY);
			if (r_open < 0)
                putErrorToBody((char *)"404", (char *)"Not found", server);
			else {
				char* temp = (char *)malloc(sizeof(char) * (bufSize));
				while ((r_read = read(r_open, temp, bufSize)) > 0)
					_bodyOfResponse.addData(temp, r_read);
				if	(r_read < 0)
					putErrorToBody((char *)"000", (char *)"The file cannot be read, bitch", server);
				free(temp);
				close(r_open);
			}
		}
	}
    else {
		this->_location_id = uriSearching(server, (char *) request.getURI().c_str());
		if (!(server.getLocations()[this->_location_id].getAllowMethods()).find(request.getMetod())->second)
			putErrorToBody((char *)"405", (char *)"Method Not Allowed", server);
        else if (check_auth(request, server.getLocations()[this->_location_id]) == -1)
            putErrorToBody((char *)"401", (char *)"Unauthorized", server);
        else
			checkFileOrDir(request, server);
    }
}

Bytes & Response::getBodyOfResponse() { return _bodyOfResponse; }

char* Response::responseGenerating(Request & request, WebServer & server) {
	Page_html page;

    _location_id = -1;
    if(request.getMetod() == "GET" || request.getMetod() == "HEAD") {
        methodGetHead(request, server, page);
        if(request.getMetod() == "HEAD") {
            _versionOfWebServer = "Server: Webserv/1.0 (MacOS)\r\n";
            std::string tmp1 = _httpVersion + _timeOfResponse + "\r\n" + _contentLength + std::to_string(_bodyOfResponse.getDataSize()) + doubleCRLF;
            _lenOfResponse = tmp1.length();
            return const_cast<char *>(ft_strdup(tmp1.c_str()));
        }
    }
    else if(request.getMetod() == "POST")
        methodPost(request, server);
    else if(request.getMetod() == "PUT")
        methodPut(request, server);
    else
        this->_httpVersion = "HTTP/1.1 400 Bad Request\r\n";
    return (editResponse(&request));
}

void Response::putErrorToBody(char *error, char *type, WebServer server)
{
	char 		*tmp;
    int         fd;
	Page_html 	errors;
	ssize_t 	r_read;

    this->_httpVersion = "HTTP/1.1 ";
    this->_httpVersion += error;
    this->_httpVersion += " ";
    this->_httpVersion += type;
    this->_httpVersion += "\n";

	std::map<std::string, std::string> error_p = server.getErrorPage();
	std::map<std::string, std::string>::iterator it = error_p.begin();
	while(it != error_p.end()) {
        if(it->first == error) {
            if((fd = open(server.getErrorPage()[error].c_str(), O_RDONLY)) < 0)
				putErrorToBody((char *)"404", (char *)"Not found", server);
            else {
				char* temp = (char *)malloc(sizeof(char) * (bufSize));
				while ((r_read = read(fd, temp, bufSize)) > 0)
					_bodyOfResponse.addData(temp, r_read);
				if	(r_read < 0)
					putErrorToBody((char *)"000", (char *)"The file cannot be read, bitch", server);
                free(temp);
                close(fd);
            }
            return ;
        }
        it++;
    }
	tmp = (char *)errors.create_error(error, type);
	_bodyOfResponse.addData(tmp, strlen(tmp));
}

char* Response::editResponse(Request *request) {
	char *		t;
	char *		tmp1;
	char *		ret;
	size_t 		size;

	if (request->getMetod() == "POST") {
		tmp1 = _bodyOfResponse.toPointer();
		if ((t = static_cast<char *>(memmem( tmp1, _bodyOfResponse.getDataSize(), "\r\n\r\n", 4)))) {
			Bytes body = _bodyOfResponse.cutData(t - tmp1 + 4);
			_bodyOfResponse.clear();
			t = body.toPointer();
			_bodyOfResponse.addData(t, body.getDataSize());
			free(t);
		}
		free(tmp1);
	}
	size = _bodyOfResponse.getDataSize();
	std::string tmp = _httpVersion + _timeOfResponse + "\r\n" +  _contentLength + std::to_string(size) + "\r\n" +  _versionOfWebServer + doubleCRLF;
	_lenOfResponse = tmp.length() + _bodyOfResponse.getDataSize();
	tmp1 = _bodyOfResponse.toPointer();
	ret = ft_memjoin((char *)tmp.c_str(), tmp1, tmp.length(), _bodyOfResponse.getDataSize());
	free(tmp1);
	return ret;
}

int Response::check_auth(Request & request, Location & location)
{
    if(location.getAuthClients().empty())
        return(0); // Не нужна авторизация для location
    else // Нужна авторизация для location
    {
        if(request.getAuthorization() == "")
            return(-1);
        for(size_t it = 0; it != location.getAuthClients().size(); it++)
            if(request.getAuthorization().substr(6, request.getAuthorization().size()) == base64_encode(location.getAuthClients()[it]))
                return(0);
    }
    return(-1);
}

size_t Response::getLenOfResponse() const { return _lenOfResponse; }
