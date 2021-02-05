/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 13:23:58 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/05 15:01:45 by atomatoe         ###   ########.fr       */
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

// О структуре tm: https://www.opennet.ru/docs/RUS/glibc/glibc-17.html
void get_time() // Date: Mon, 27 Jul 2009 12:28:53 GMT - делает такую строку для ответа за запрос клиента
{
    struct timeval tv;
    struct timezone tz;
    struct tm tm; // здесь будет наша дата лежать
    int sec_per_day = 86400;
    int sec_per_hour = 3600;
    int sec_per_min = 60;
    gettimeofday(&tv, &tz);

    time_t tmp = tv.tv_sec;
    int flag = 4;
    int year = 0;
    // tmp = tmp - 31536000 - 31536000;
    while(tmp > 31536000)
    {
        if(flag == 4)
        {
            tmp -= 31622400; 
            flag = 0;
        }
        else
        {
            tmp -= 31536000;
            flag++;
        }
        year++;
    }
    tmp -= 75600; // погрешность
    tm.tm_mon = 0;
    while(tmp > 2419200) // месяца
    {
        if(tm.tm_mon == 0) { tmp -= 2678400; tm.tm_mon++; } // январь
        else if(tm.tm_mon == 1) // февраль (может быть высокосным)
        {
            if(flag == 4)
                tmp -= 2505600;
            else
                tmp -= 2419200;
            tm.tm_mon++;
        }
        else if(tm.tm_mon == 2) { tmp -= 2678400; tm.tm_mon++; } // март
        else if(tm.tm_mon == 3) { tmp -= 2592000; tm.tm_mon++; } // апрель
        else if(tm.tm_mon == 4) { tmp -= 2678400; tm.tm_mon++; } // май
        else if(tm.tm_mon == 5) { tmp -= 2592000; tm.tm_mon++; } // июнь
        else if(tm.tm_mon == 6) { tmp -= 2678400; tm.tm_mon++; } // июль
        else if(tm.tm_mon == 7) { tmp -= 2678400; tm.tm_mon++; } // август
        else if(tm.tm_mon == 8) { tmp -= 2592000; tm.tm_mon++; } // сентябрь
        else if(tm.tm_mon == 9) { tmp -= 2678400; tm.tm_mon++; } // октябрь
        else if(tm.tm_mon == 10) { tmp -= 2592000; tm.tm_mon++; } // ноябрь
        else if(tm.tm_mon == 11) { tmp -= 2678400; tm.tm_mon++; } // декабрь
    }
    tm.tm_mday = 0;
    while(tmp > 86400) // дни
    {
        tmp -= 86400;
        tm.tm_mday++;
    }
    tm.tm_year = 70 + year; // получаем год

    long hms = tv.tv_sec % sec_per_day;
    hms += tz.tz_dsttime * sec_per_hour;
    hms -= tz.tz_minuteswest * sec_per_min;
    hms = (hms + sec_per_day) % sec_per_day;

    tm.tm_hour = hms / sec_per_hour; // получение часа
    tm.tm_min = (hms % sec_per_hour) / sec_per_min; // получение минут
    tm.tm_sec = (hms % sec_per_hour) % sec_per_min; // получение секунд
    char zon[4] = "GMT";
    tm.tm_zone = zon;
    long test = tv.tv_sec / 86400 ;
    test = test % 7; // какой день недели
    tm.tm_wday = test + 4;


    char buf[200];
    tm.tm_isdst = -1; // не используем(переход летнее время)
    tm.tm_gmtoff = 0; // не используем
    tm.tm_yday = 0; // не используем
    strftime (buf, 200, "Date: %a, %d %b %Y %X %Z", &tm);
    std::cout << buf << std::endl;
}

#endif
  