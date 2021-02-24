/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 16:10:54 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/20 16:16:54 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response() {
	this->_httpVersion = "HTTP/1.1 200 OK\r\n";
	struct timeval tv;
	gettimeofday(&tv, NULL);
	this->_timeOfResponse = get_time(tv.tv_sec);
	this->_versionOfWebServer = "Server: Webserv/1.0 (MacOS)\r\n\r\n";
	this->_location_id = 0;
	_lenOfResponse = 0;
}
Response::~Response() { }

int Response::search_uri(WebServer server, char *uri) {
	size_t it = 0;

	while(it != server.getLocations().size())
	{
		if ((server.getLocations()[it].getUrl()) == uri) // проверка на существование такого location по запросу
		{
			this->_location_id = it;
			return(0);
		}
		it++;
	}
	return(-1);
}

int Response::check_uri(WebServer server, char *uri) {
	struct stat sb;
	size_t i = strlen(uri);
	size_t t = 0;
	int ret;

	if(stat(uri, &sb) == 0 && S_ISDIR(sb.st_mode)) {
		std::cout << "testers" << std::endl;
		return (-1);
	}
	else {
		while(uri[i] != '/')
			i--;
		char* tmp = (char *)malloc(sizeof(char) * (i + 2));
		while(t != i) {
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

void Response::putErrorToBody(char *error, char *type) {
	char 		*tmp;
	Page_html errors; // для генерации ошибок

	tmp = errors.create_error(error, type);
	_bodyOfResponse.addData(tmp, ft_strlen(tmp));
}

char* Response::give_me_response(Request request, WebServer server) {

	std::string directory;
	Page_html autoindex;
	char *tmp;

	/*CGI calling*/

	if (strcmp(request.getMetod(), "POST") == 0) {
//		request.setPathToCgi(std::string("/Users/welease/projects/webserv/testing_cgi/cgi-bin/html.py"));
//		toCGI(*this, request, server);
//		//std::cout << "+++" << _bodyOfResponse.toPointer() << "+++" << _bodyOfResponse.getDataSize() << "+++" << std::endl;
//		tmp = _bodyOfResponse.toPointer();
//		this->_versionOfWebServer = "Server: Webserv/1.0 (MacOS)";
//		return (edit_response());

		if(search_uri(server, request.getURI()) == -1 && check_uri(server, request.getURI()) == -1)
		{
			this->_httpVersion = "HTTP/1.1 404 Not Found\n"; // std::cout << "такого location не существует" << std::endl;
			if(server.getErrorPage().find((char *)"404")->first == server.getErrorPage().end()->first) // std::cout << "error page отсутствует" << std::endl;
				putErrorToBody((char *)"404", (char *)"Not Found");
			else // std::cout << "error page присутствует" << std::endl;
			{
				int fd_tmp = open(server.getErrorPage().find((char *)"404")->second.c_str(), O_RDONLY);
				if(fd_tmp < 0)
				{
					this->_httpVersion = "HTTP/1.1 404 Not Found\n";
					putErrorToBody((char *)"404", (char *)"Not Found");
				}
				else
				{
					char* temp = (char *)malloc(sizeof(char) * 4097);
					int i = read(fd_tmp, temp, 5555);
					if(i < 0)
						putErrorToBody((char *)"000", (char *)"The file cannot be read, bitch");
					else
						_bodyOfResponse.addData(temp, ft_strlen(temp));
					close(fd_tmp);
					free(temp);
				}
			}
		}
		else
		{
			if (!(server.getLocations()[this->_location_id].getAllowMethods()).find("POST")->second)
			{
				this->_httpVersion = "HTTP/1.1 405 Method Not Allowed\n"; // std::cout << "такой метод запрещен!" << std::endl;
				if (server.getErrorPage().find((char *) "405")->first == server.getErrorPage().end()->first) // std::cout << "error page отсутствует" << std::endl;
					putErrorToBody((char *) "405", (char *) "Method Not Allowed");
				else // std::cout << "error page присутствует" << std::endl;
				{
					int fd_tmp = open(server.getErrorPage().find((char *) "405")->second.c_str(), O_RDONLY);
					if (fd_tmp < 0)
					{
						this->_httpVersion = "HTTP/1.1 404 Not Found\n";
						putErrorToBody((char *) "404", (char *) "Not Found");
					} else
					{
						char *temp = (char *) malloc(sizeof(char) * 4097);
						int i = read(fd_tmp, temp, 5555);
						if (i < 0)
							putErrorToBody((char *) "12", (char *) "The file cannot be read, bitch");
						else if (i < 0)
							putErrorToBody((char *) "000", (char *) "The file cannot be read, bitch");
						else
							_bodyOfResponse.addData(temp, ft_strlen(temp));
						close(fd_tmp);
						free(temp);
					}
				}
			} else {
				request.setPathToCgi(std::string("/Users/welease/projects/webserv/testing_cgi/cgi-bin/html.py"));
				toCGI(*this, request, server);
				//std::cout << "+++" << _bodyOfResponse.toPointer() << "+++" << _bodyOfResponse.getDataSize() << "+++" << std::endl;
				tmp = _bodyOfResponse.toPointer();
				this->_versionOfWebServer = "Server: Webserv/1.0 (MacOS)";
				return (edit_response());
			}
		}
	}

	/*end of CGI*/

	if(strcmp(request.getMetod(), "GET") == 0) {
		if(search_uri(server, request.getURI()) == -1 && check_uri(server, request.getURI()) == -1) {
			this->_httpVersion = "HTTP/1.1 404 Not Found\n"; // std::cout << "такого location не существует" << std::endl;
			std::cout << server.getErrorPage().find((char *)"404")->first << std::endl;
			if(server.getErrorPage().find((char *)"404")->first == server.getErrorPage().end()->first)// std::cout << "error page отсутствует" << std::endl;
				putErrorToBody((char *)"404", (char *)"Not found");
			else {
				int fd_tmp = open(server.getErrorPage().find((char *)"404")->second.c_str(), O_RDONLY);
				if(fd_tmp < 0) {
					this->_httpVersion = "HTTP/1.1 404 Not Found\n";
					putErrorToBody((char *)"404", (char *)"Not found");
				}
				else {
					char* temp = (char *)malloc(sizeof(char) * 4097);
					int i = read(fd_tmp, temp, 5555);
					if(i < 0)
						putErrorToBody((char *)"000", (char *)"The file cannot be read, bitch");
					else
						_bodyOfResponse.addData(temp, ft_strlen(temp));
					free(temp);
					close(fd_tmp);
				}
			}
		}
		else {
			if (!(server.getLocations()[this->_location_id].getAllowMethods()).find("GET")->second) {
				this->_httpVersion = "HTTP/1.1 405 Method Not Allowed\n"; // std::cout << "такой метод запрещен!" << std::endl;
				if(server.getErrorPage().find((char *)"405")->first == server.getErrorPage().end()->first) // std::cout << "error page отсутствует" << std::endl;
					putErrorToBody((char *)"405", (char *)"Method Not Allowed");
				else // std::cout << "error page присутствует" << std::endl;
				{
					int fd_tmp = open(server.getErrorPage().find((char *)"405")->second.c_str(), O_RDONLY);
					if(fd_tmp < 0) {// File not open
						this->_httpVersion = "HTTP/1.1 404 Not Found\n";
						putErrorToBody((char *)"404", (char *)"Not Found");
					}
					else {
						char* temp = (char *)malloc(sizeof(char) * 4097);
						int i = read(fd_tmp, temp, 5555);
						if(i < 0)
							putErrorToBody((char *)"000", (char *)"The file cannot be read, bitch");
						else
							_bodyOfResponse.addData(temp, ft_strlen(temp));
						close(fd_tmp);
						free(temp);
					}
				}
			}
			else
			{
				if(server.getLocations()[this->_location_id].getIndex() == "" &&
				   !server.getLocations()[this->_location_id].isAutoIndex())
				{
					this->_httpVersion = "HTTP/1.1 204 No Content\n";
					putErrorToBody((char *)"204", (char *)"No Content");
				}
				else
				{
					struct stat sb;
					if (stat(request.getURI(), &sb) == 0 && S_ISDIR(sb.st_mode))
					{
						if(server.getLocations()[this->_location_id].isAutoIndex() == true) {
							tmp = autoindex.create_autoindex(server.getLocations()[this->_location_id].getRoot() + request.getURI());
							_bodyOfResponse.addData(tmp, ft_strlen(tmp));
						}
						else
						{
							directory = server.getLocations()[this->_location_id].getIndex();
							int fd_tmp = open(directory.c_str(), O_RDONLY);
							if(fd_tmp < 0) // file not open
							{
								this->_httpVersion = "HTTP/1.1 404 Not Found\n";
								putErrorToBody((char *)"404", (char *)"Not Found");
							}
							else {
								char* temp = (char *)malloc(sizeof(char) * 2000000000);
								int i = read(fd_tmp, temp, 2000000000);
								if(i < 0)
									putErrorToBody((char *)"000", (char *)"The file cannot be read, bitch");
								else
									_bodyOfResponse.addData(temp, ft_strlen(temp));
								close(fd_tmp);
								free(temp);
							}
						}
					}
					else
					{
						directory = server.getLocations()[this->_location_id].getRoot() + request.getURI();
						if (stat(directory.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) {
							tmp = autoindex.create_autoindex(directory.c_str());
							_bodyOfResponse.addData(tmp, ft_strlen(tmp));
						}
						else
						{
							std::cout << directory << std::endl;
							int fd_tmp = open(directory.c_str(), O_RDONLY);
							if(fd_tmp < 0) {
								this->_httpVersion = "HTTP/1.1 404 Not Found\n";
								putErrorToBody((char *)"404", (char *)"Not Found");
							}
							else {
								std::cout << "PICTURE\n";
								char* temp = (char *)malloc(sizeof(char) * 20000000);
								int i = read(fd_tmp, temp, 20000000);
								std::cout << "size: " << i << std::endl;
								if(i < 0)
									putErrorToBody((char *)"000", (char *)"The file cannot be read, bitch");
								else {
									std::cout << "HERER\n";
									_bodyOfResponse.addData(temp, i);
								}
								close(fd_tmp);
								free(temp);
							}
						}
					}
				}
			}
		}
	}
	else if(strcmp(request.getMetod(), "PUT") == 0)
	{
		if(search_uri(server, request.getURI()) == -1 && check_uri(server, request.getURI()) == -1)
		{
			this->_httpVersion = "HTTP/1.1 404 Not Found\n"; // std::cout << "такого location не существует" << std::endl;
			if(server.getErrorPage().find((char *)"404")->first == server.getErrorPage().end()->first) // std::cout << "error page отсутствует" << std::endl;
				putErrorToBody((char *)"404", (char *)"Not Found");
			else // std::cout << "error page присутствует" << std::endl;
			{
				int fd_tmp = open(server.getErrorPage().find((char *)"404")->second.c_str(), O_RDONLY);
				if(fd_tmp < 0)
				{
					this->_httpVersion = "HTTP/1.1 404 Not Found\n";
					putErrorToBody((char *)"404", (char *)"Not Found");
				}
				else
				{
					char* temp = (char *)malloc(sizeof(char) * 4097);
					int i = read(fd_tmp, temp, 5555);
					if(i < 0)
						putErrorToBody((char *)"000", (char *)"The file cannot be read, bitch");
					else
						_bodyOfResponse.addData(temp, ft_strlen(temp));
					close(fd_tmp);
					free(temp);
				}
			}
		}
		else
		{
			if (!(server.getLocations()[this->_location_id].getAllowMethods()).find("PUT")->second)
			{
				this->_httpVersion = "HTTP/1.1 405 Method Not Allowed\n"; // std::cout << "такой метод запрещен!" << std::endl;
				if(server.getErrorPage().find((char *)"405")->first == server.getErrorPage().end()->first) // std::cout << "error page отсутствует" << std::endl;
					putErrorToBody((char *)"405", (char *)"Method Not Allowed");
				else // std::cout << "error page присутствует" << std::endl;
				{
					int fd_tmp = open(server.getErrorPage().find((char *)"405")->second.c_str(), O_RDONLY);
					if(fd_tmp < 0)
					{
						this->_httpVersion = "HTTP/1.1 404 Not Found\n";
						putErrorToBody((char *)"404", (char *)"Not Found");
					}
					else
					{
						char* temp = (char *)malloc(sizeof(char) * 4097);
						int i = read(fd_tmp, temp, 5555);
						if(i < 0)
							putErrorToBody((char *)"12", (char *)"The file cannot be read, bitch");
						else
						if(i < 0)
							putErrorToBody((char *)"000", (char *)"The file cannot be read, bitch");
						else
							_bodyOfResponse.addData(temp, ft_strlen(temp));
						close(fd_tmp);
						free(temp);
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
					buff += tmp[it++];
				std::string str = server.getLocations()[this->_location_id].getRoot() + request.getURI();
				std::ofstream filename(str, std::ios::app); // std::ios::app - для записи в конец файла
				if(!filename)
					std::cout << "open file error!" << std::endl;
				filename << buff;
				filename.close();
			}
		}
	}
	else if(strcmp(request.getMetod(), "HEAD") == 0)
	{
		if(search_uri(server, request.getURI()) == -1 && check_uri(server, request.getURI()) == -1)
			this->_httpVersion = "HTTP/1.1 404 Not Found\n";
		if (!(server.getLocations()[this->_location_id].getAllowMethods()).find("PUT")->second)
			this->_httpVersion = "HTTP/1.1 405 Method Not Allowed\n";
	}
	else if((strcmp(request.getMetod(), "GET") != 0) && (strcmp(request.getMetod(), "PUT") != 0) && (strcmp(request.getMetod(), "POST") != 0)
			&& (strcmp(request.getMetod(), "GET") != 0) && (strcmp(request.getMetod(), "HEAD") != 0))
		this->_httpVersion = "HTTP/1.1 400 Bad Request\n";
	return(edit_response());
}

char* Response::edit_response() {
	size_t size;
	size = _httpVersion.length() + _timeOfResponse.length() + _versionOfWebServer.length() + 2;
	std::string tmp = _httpVersion + _timeOfResponse + "\r\n" + _versionOfWebServer;
	//_bodyOfResponse.addData((char *)"", 1);
	_lenOfResponse = size + _bodyOfResponse.getDataSize();
	return (ft_memjoin((char *)tmp.c_str(), _bodyOfResponse.toPointer(), size, _bodyOfResponse.getDataSize()));
}

size_t Response::getLenOfResponse() const {
	return _lenOfResponse;
}