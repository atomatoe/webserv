/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 14:02:33 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/17 18:58:57 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

WebServer::WebServer() : _port(-1), _autoIndex(false) { }

WebServer::~WebServer()
{
    
}

void WebServer::create_socket()
{
    this->_server_fd = socket(AF_INET, SOCK_STREAM, 0);
    this->_address_len = (sizeof(this->_socket_addr));
    this->_socket_addr.sin_family = AF_INET;
    this->_socket_addr.sin_addr.s_addr = inet_addr(this->_ip.c_str()); // содержит адрес (номер) узла сети.
    this->_socket_addr.sin_port = htons(this->_port); // 80 - port с конфига  что делает htons (80 >> 8 | 80 << 8)
}

int WebServer::build_server()
{
    if(setsockopt(this->_server_fd, SOL_SOCKET, SO_REUSEADDR, &this->_yes, sizeof(int))) // дает возможность повторно использовать сокет (повторять bind)
    {
        std::cerr << "setsockopt FAILED" << std::endl;
        return(-1);
    }
    if(bind(this->_server_fd,(const struct sockaddr *) &this->_socket_addr, this->_address_len)) // Аргумент address_len задает размер (в байтах) структуры данных, указываемой аргументом addr.
    {
        std::cerr << "bind FAILED" << std::endl;
        return(-1);
    }
    if(fcntl(this->_server_fd, F_SETFL, O_NONBLOCK)) // делаем сокет неблокирующемся
    {
        std::cerr << "fcntl FAILED" << std::endl;
        return(-1);
    }
    if(listen(this->_server_fd, 5)) {
        std::cerr << "listen FAILED" << std::endl;
        return(-1);
    }
    return(0);
}

int WebServer::get_server_fd() { return(this->_server_fd); }
sockaddr_in *WebServer::get_socket_addr() { return(&this->_socket_addr); }
socklen_t *WebServer::get_address_len() { return(&this->_address_len); }
sockaddr_in *WebServer::get_out() { return(&this->_out); }
std::map<int, t_client> &WebServer::get_list() { return(this->_mapOfClients); }


////////////////* from Timur */
/* get */
std::string                         WebServer::getIp() const { return _ip; }
int                                 WebServer::getPort() const { return _port; }
std::string                         WebServer::getServerName() const { return _serverName; }
std::string                         WebServer::getRootPath() const { return _rootPath; }
std::map<std::string, std::string>  WebServer::getErrorPage() const { return _errorPage; };
std::vector<Location>               WebServer::getLocations() const {
    return _locations;
}

/* set */
void            WebServer::setIp(std::string ip) {
    if (ip[0] == '.' || ip[ip.size() - 1] == '.')
        exitError("Incorrect value of ip: \"" + ip + "\"");
    char **splitted = ft_splitTim(ip.c_str(), '.');
    if (ft_strstrlen(splitted) != 4)
        exitError("Incorrect value of ip: \"" + ip + "\"");
    for (int i = 0; i < 4 ; i ++)
        if (!ft_str_is_num(splitted[i]))
            exitError("Incorrect symbols in ip value: \"" + ip + "\"");
    ft_free_strstr(splitted);
    this->_ip = ip;
}
void            WebServer::setPort(int port) {
    this->_port = port;
}
void            WebServer::setServerName(std::string serverName) {
    this->_serverName = serverName;
}
void            WebServer::setRootPath(std::string rootPath) {
    if (!isDirectory(rootPath))
        exitError("Smth wrong with root value: \"" + rootPath + "\"");
    this->_rootPath = rootPath;
}

/* add */
void            WebServer::addErrorPage(std::string error, std::string path) {
    if (!ft_str_is_num(error.c_str()))
        exitError("Incorrect value in error_page \"" + error + "\"");
    if (!isFileRead(path))
        exitError("Incorrect value in error_page \"" + path + "\"");
    _errorPage.insert(std::pair<std::string, std::string>(error, path));
}
void            WebServer::addLocation(Location location) {
    this->_locations.push_back(location);
}

void WebServer::addClient(int fd){
	t_client        tmp;
	struct timeval  tv;

	gettimeofday(&tv, NULL);
	_mapOfClients.insert(std::make_pair(fd, tmp));
    _mapOfClients[fd].time = tv.tv_sec;
	_mapOfClients[fd].toSendData = new Bytes();
	_mapOfClients[fd].receivedData = new Bytes();
	_mapOfClients[fd].phase = 0;
	_mapOfClients[fd].sendBytes = 0;
	fcntl(fd, F_SETFL, O_NONBLOCK);
}

std::map<int, t_client> &WebServer::getClients() {
	return _mapOfClients;
}

bool  WebServer::getAutoIndex() const { return _autoIndex; }
void  WebServer::setAutoIndex(bool autoIndex) { _autoIndex = autoIndex; }