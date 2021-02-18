/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 13:23:58 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/16 18:16:10 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_HPP
# define INCLUDES_HPP

#include <iostream> // std::
#include <sys/socket.h> // для socket
#include <netinet/in.h> // для sockaddr_in
#include <arpa/inet.h> // для inet_addr
#include <fcntl.h> // для fcntl
#include <unistd.h> // для close, write, open и другой хуеты
#include <sys/select.h> // для макросов с работой select FD_CLR, FD_ISSET, FD_SET
#include <map> // map STL
#include <vector> // vector STL
#include <stdlib.h> // для маллок
#include <sys/time.h> // для gettimeofday
#include <dirent.h> // для readdir opendir
#include <sys/stat.h> // для lstat stat
#include <fstream> // для работы с файлами ofstream

char    *str_join(char *buf, char *add);
char*   get_time(time_t time);
void    *ft_memset(void *src, int g, size_t l);



#endif
  