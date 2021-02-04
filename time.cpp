/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 17:39:26 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/04 19:11:39 by atomatoe         ###   ########.fr       */
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

    

    long hms = tv.tv_sec % sec_per_day;
    hms += tz.tz_dsttime * sec_per_hour;
    hms -= tz.tz_minuteswest * sec_per_min;
    hms = (hms + sec_per_day) % sec_per_day;

    tm.tm_year = 70 + (tv.tv_sec / 31536000); // получение года
    tm.tm_hour = hms / sec_per_hour; // получение часа
    tm.tm_min = (hms % sec_per_hour) / sec_per_min; // получение минуты
    tm.tm_sec = (hms % sec_per_hour) % sec_per_min; // получение секунды

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
    // tm.tm_year = 0;
    // tm.tm_hour = 0;
    // tm.tm_wday = 0;
    // tm.tm_min = 0;
    // tm.tm_sec = 0;
    strftime (buf, 200, "Date: %a, %d %b %Y %X %Z", &tm);
    std::cout << buf << std::endl;
    std::cout << "Date: Mon, 27 Jul 2009 12:28:53 GMT" << std::endl;
    // printf("Date: %d %d:%02d:%02d\n", tm.tm_year, tm.tm_hour, tm.tm_min, tm.tm_sec);
    return 0;
}