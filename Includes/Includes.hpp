/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 13:23:58 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/16 18:16:10 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_HPP
# define INCLUDES_HPP
# define DEFAULT "\e[39m\e[0m"
# define GREEN "\e[92m"

#include <iostream> // std::
#include <sys/socket.h> // для socket
#include <netinet/in.h> // для sockaddr_in
#include <arpa/inet.h> // для inet_addr
#include <fcntl.h> // для fcntl
#include <unistd.h> // для close, write, open и другой хуеты
#include <sys/select.h> // для макросов с работой select FD_CLR, FD_ISSET, FD_SET
#include <map> // map STL
#include <vector> // vector STL
#include <list> // list STL
#include <stdlib.h> // для маллок
#include <sys/time.h> // для gettimeofday
#include <dirent.h> // для readdir opendir
#include <sys/stat.h> // для lstat stat
#include <fstream> // для работы с файлами ofstream
#define doubleCRLF (char *)"\r\n\r\n"

class Location;
class ParseConfig;
class WebServer;
class Page_html;
#include "../Sources/ParseConfig/Location.hpp"
#include "../Sources/ParseConfig/ParseConfig.hpp"
#include "../GetNextLine/get_next_line.hpp"
#include "../Libft/libft.h"
#include "../Sources/PageHtmlGeneration/PageHtml.hpp"


char*   get_time(time_t time);
void    *ft_memset(void *src, int g, size_t l);

void    errorMalloc();
void    init_servers(std::vector<WebServer> & servers, size_t count_server);
void	exitSignal(int signalNum);
void 	start_servers(std::vector<WebServer> & servers);

#endif
  