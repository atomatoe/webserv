/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 13:23:58 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/03 16:57:48 by atomatoe         ###   ########.fr       */
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
#include <map> // угадай для чего
#include <stdlib.h> // для маллок
#include <sys/time.h> // для gettimeofday

char *str_join(char *buf, char *add)
{
	char	*newbuf;
	int		len; 
	if (buf == 0)
		len = 0;
	else
		len = strlen(buf);
	newbuf = (char *)malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
	if (newbuf == 0)
		return (0);
	newbuf[0] = 0;
	if (buf != 0)
		strcat(newbuf, buf);
	free(buf);
	strcat(newbuf, add);
	return (newbuf);
}

#endif
  