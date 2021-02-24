/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   page_html.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 15:48:01 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/24 17:50:03 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "page_html.hpp"

char* Page_html::get_way(std::string s1, char *s2)
{
    char *str = NULL;
    unsigned int i = s1.size();
    unsigned int t = strlen(s2);
    str = (char *)malloc(sizeof(char) * (i + t + 2));
    i = 0; t = 0;
    while(s1[i] != '\0')
    {
        str[i] = s1[i];
        i++;
    }
    // str[i] = '/';
    // i++;
    while(s2[t] != '\0')
    {
        str[i] = s2[t++];
        i++;
    }
    str[i] = '\0';
    return(str);
}

Page_html::Page_html()
{ 
    _dir = NULL;
    _file = NULL;
    ft_memset(&buff, '\0', sizeof(buff));
}

Page_html::~Page_html() { };

const char* Page_html::create_autoindex(std::string _directory, char *uri)
{
    _dir = opendir(_directory.c_str());

    _file = readdir(_dir);
    _html += (char *)"<table style=\"width: 50%; border-collapse: collapse; margin-left: auto; margin-right: auto; height: 162px;\" border=\"1\" bgcolor=\"#9442ad\">\n<tbody>\n<tr style=\"height: 108px;\">\n<td bgcolor=\"#59bd59\" style=\"width: 50.25%; height: 108px; text-align: center;\">\n<p><strong>&#128149;<a href=\"https://github.com/atomatoe/webserv\">Welcome to Webserv!</a>&#128149;</strong></p>\n<p>Index of ";
    _html += _directory;
    _html += (char *)"</p>\n</td>\n<td style=\"width: 49.75%; height: 150px; text-align: center;\">\n<p><strong>Creators:</strong></p>\n<p>&#128125; <a href=\"https://github.com/atomatoe\"> atomatoe </a>&#128125;</p>\n<p><a><span>&#128123; <a href=\"https://github.com/Welease\"> welease </a>&#128123;</span></a></p>\n<p><a><span>&#129302; <a href=\"https://github.com/timurmug\">qtamaril</a> &#129302;</span></a></p>\n</td>\n</tr>\n";
    while((_file = readdir(_dir)))
    {
        _html += (char *)"<tr style=\"height: 18px;\"><td style=\"width: 50.25%; text-align: center; height: 30px;\" bgcolor=\"#1a7d99\"><a href=\"";
        std::cout << "dir autoindex = " << _directory << std::endl;
        std::cout << "file autoindex = " << _file->d_name << std::endl;
        std::cout << "uri autoindex = " << uri << std::endl;
        _html += uri;
        if(strcmp(uri, "/") != 0)
            _html += "/";
        _html += _file->d_name;
        _html += (char *)"\">";
        _html += _file->d_name;
        if (_file->d_type == 4)
			_html += "/";
        stat(get_way(_directory, _file->d_name), &buff); // нужен полный путь до файла
        _html += (char *)"</a></td>\n<td style=\"width: 49.75%; height: 30px; text-align: center;\" bgcolor=\"#1a7d99\">";
        _html += get_time(buff.st_mtimespec.tv_sec);
        _html += (char *)"</td>\n</tr>";
    }
    _html += (char *)"</tbody>\n</table>";
    closedir(_dir);
    return(_html.c_str());
}

const char* Page_html::create_error(char* code, char* text) // создаст ошибку с № и описанием. Сформирует ее в виде html, вернет html код в char*.
{
    _html += (char *)"<table style=\"width: 61.125%; border-collapse: collapse; margin-left: auto; margin-right: auto; height: 277px;\" border=\"1\" bgcolor=\"#9442ad\">\n<tbody>\n<tr style=\"height: 108px;\">\n<td bgcolor=\"#b5283e\" style=\"width: 50.25%; height: 139px; text-align: center;\">\n<h2>Welcome to Webserv!</h2>\n</td>\n<td style=\"width: 49.75%; height: 139px; text-align: center;\" scope=\"col\">\n<h2>Creators:</h2>\n<p>&#129418;<a href=\"https://github.com/atomatoe\"> atomatoe </a>&#129415;</p>\n<p>&#129412;<a href=\"https://github.com/Welease\"><span> welease </span></a>&#129433;</p>\n<p>&#128005;<a href=\"https://github.com/timurmug\"><span> qtamaril </span></a>&#129423;</p>\n</td>\n</tr>\n<tr style=\"height: 18px;\">\n<td style=\"width: 50.25%; height: 138px; text-align: center;\" bgcolor=\"#b5283e\">\n<h2>";
    _html += code;
    _html += (char *)"</h2>\n<p>";
    _html += text;
    _html += (char *)" &#128532;</p></tbody>\n</table>\n<p style=\"text-align: center;\"><small>webserv/1.0</small></p>";
    return(_html.c_str());
}