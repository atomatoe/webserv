/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 17:39:26 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/03 19:29:18 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"

// Date: Mon, 27 Jul 2009 12:28:53 GMT   вот так ответ должен быть
// результат поместить либо в char* либо в std::string

int main()
{
    struct timeval tv;
    struct timezone tz;
    struct tm tm; // здесь будет наша дата лежать
    int sec_per_day = 86400;
    int sec_per_hour = 3600;
    int sec_per_min = 60;
    gettimeofday(&tv, &tz);
    // printf("TimeZone-1 = %d\n", tz.tz_minuteswest);
    // printf("TimeZone-2 = %d\n", tz.tz_dsttime);
    // printf("TimeVal-3  = %lld\n", (long long) tv.tv_sec);
    // printf("TimeVal-4  = %lld\n", (long long) tv.tv_usec);
    long hms = tv.tv_sec % sec_per_day;
    hms += tz.tz_dsttime * sec_per_hour;
    hms -= tz.tz_minuteswest * sec_per_min;
    hms = (hms + sec_per_day) % sec_per_day;

    tm.tm_mon = (tv.tv_sec - 1609448401); // получение месяца
    tm.tm_year = 1970 + (tv.tv_sec / 31536000); // получение даты
    tm.tm_hour = hms / sec_per_hour;
    tm.tm_min = (hms % sec_per_hour) / sec_per_min;
    tm.tm_sec = (hms % sec_per_hour) % sec_per_min;

    printf("Current local time: %d %d:%02d:%02d\n", tm.tm_year, tm.tm_hour, tm.tm_min, tm.tm_sec);
    return 0;
}