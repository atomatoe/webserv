/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 17:39:26 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/04 21:05:21 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"

// Date: Mon, 27 Jul 2009 12:28:53 GMT   вот так ответ должен быть
// результат поместить либо в char*
// О структуре tm: https://www.opennet.ru/docs/RUS/glibc/glibc-17.html

int main()
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
    tmp = tmp - 31536000 - 31536000;
    while(tmp > 31622400)
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
    tmp -= 2678400;
    tmp -= 86400;
    tmp -= 86400;
    tmp -= 86400;
    tmp -= 86400;
    std::cout << tmp << std::endl;
    // tmp += 31536000; // переходим на 1 год вперед
    // while(tmp < 0)
    // {
    //     2419200
    // }
    // tmp += 2678400;
    // tmp += 2678400;
    // tmp += 2678400;
    // tmp += 2678400;
    // tmp += 2678400;
    // tmp += 2678400;
    // tmp += 2592000;
    // tmp += 2592000;
    // tmp += 2592000;
    // tmp += 2592000;
    // tmp += 2419200;
    // tmp -= 86400;
    // tmp -= 86400;
    // tmp -= 86400;
    // tmp -= 86400;
    // tmp -= 86400;
    // tmp -= 86400;
    // tmp -= 86400;
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
    tm.tm_mon = 0; // день
    tm.tm_mday = 0; // месяц
    strftime (buf, 200, "Date: %a, %d %b %Y %X %Z", &tm);
    std::cout << buf << std::endl;
    std::cout << "Date: Mon, 27 Feb 2021 12:28:53 GMT" << std::endl;
    return 0;
}


    // tm.tm_year = 0;
    // tm.tm_hour = 0;
    // tm.tm_wday = 0;
    // tm.tm_min = 0;
    // tm.tm_sec = 0;