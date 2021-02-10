/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   page_html.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 15:48:01 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/10 20:04:48 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "page_html.hpp"

char* Page_html::get_way(char *s1, char *s2)
{
    char *str = NULL;
    unsigned int i = strlen(s1);
    unsigned int t = strlen(s2);
    str = (char *)malloc(sizeof(char) * (i + t + 2));
    i = 0; t = 0;
    while(s1[i] != '\0')
    {
        str[i] = s1[i];
        i++;
    }
    str[i] = '/';
    i++;
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
    _html = NULL;
    _dir = NULL;
    _file = NULL;
    ft_memset(&buff, '\0', sizeof(buff));
}

Page_html::~Page_html() { };

char* Page_html::create_autoindex(char * _directory)
{
    char *tmp;
    _dir = opendir(_directory);
    _file = readdir(_dir);
    _html = str_join(_html, (char *)"<table style=\"width: 50%; border-collapse: collapse; margin-left: auto; margin-right: auto; height: 162px;\" border=\"1\" bgcolor=\"#9442ad\">\n<tbody>\n<tr style=\"height: 108px;\">\n<td bgcolor=\"#59bd59\" style=\"width: 50.25%; height: 108px; text-align: center;\">\n<p><strong>&#128149;Welcome to Webserv!&#128149;</strong></p>\n<p>Index of ");
    _html = str_join(_html, (char *)"/TEST! this is block for ROOT folder");
    _html = str_join(_html, (char *)"</p>\n</td>\n<td style=\"width: 49.75%; height: 150px; text-align: center;\">\n<p><strong>Creators:</strong></p>\n<p>&#128125; atomatoe &#128125;</p>\n<p><a><span>&#128123; welease &#128123;</span></a></p>\n<p><a><span>&#129302; qtamaril &#129302;</span></a></p>\n</td>\n</tr>\n");
    while((_file = readdir(_dir)))
    {
        _html = str_join(_html, (char *)"<tr style=\"height: 18px;\"><td style=\"width: 50.25%; text-align: center; height: 30px;\" bgcolor=\"#1a7d99\"><a href=\"");
        _html = str_join(_html, _file->d_name);
        _html = str_join(_html, (char *)"\">");
        _html = str_join(_html, _file->d_name);
        if (_file->d_type == 4)
			_html = str_join(_html, (char *)"/");
        stat(get_way(_directory, _file->d_name), &buff); // нужен полный путь до файла
        _html = str_join(_html, (char *)"</a></td>\n<td style=\"width: 49.75%; height: 30px; text-align: center;\" bgcolor=\"#1a7d99\">");
        _html = str_join(_html, get_time(buff.st_mtimespec.tv_sec));
        _html = str_join(_html, (char *)"</td>\n</tr>");
    }
    _html = str_join(_html, (char *)"</tbody>\n</table>");
    closedir(_dir);
    return(_html);
    // char *tmp;
    // _dir = opendir(_directory);
    // _file = readdir(_dir);
    // _html = str_join(_html, (char *)"<div>\n<table border=\"2\" align=\"center\" rules=\"rows\">\n<tbody>\n<tr>\n<td>\n<table border=\"60\" bgcolor=\"#c9ce29\" cellpadding=\"60\">\n<tbody>\n<tr>\n<th>\n<h2>WEBSERV</h2>\n<h4>project</h4>\n</th>\n</tr>\n<tr>\n<td rowspan=\"2\">\n<h3>Index of ");
    // _html = str_join(_html, _directory);
    // _html = str_join(_html, (char *)"</h3>\n");
    // while((_file = readdir(_dir)))
    // {
    //     _html = str_join(_html, (char *)"<a href=\"");
    //     _html = str_join(_html, _file->d_name);
    //     _html = str_join(_html, (char *)"\">");
    //     _html = str_join(_html, _file->d_name);
    //     if (_file->d_type == 4)
	// 		_html = str_join(_html, (char *)"/");
    //     stat(get_way(_directory, _file->d_name), &buff); // нужен полный путь до файла
    //     _html = str_join(_html, get_time(buff.st_mtimespec.tv_sec));
    //     _html = str_join(_html, (char *)"</a><br>");
    //     std::cout << _html << std::endl;
    // }
    // _html = str_join(_html, (char *)"</td>\n<td bgcolor=\"#a5eda5\">\n<h4>Creators:</h4>\n<p><a><span>atomatoe</span></a></p>\n<p><a> <span>welease</span></a></p>\n<p><a><span>qtamaril </span></a></p>\n</td>\n</div>\n");
    // closedir(_dir);
    // return(_html);
}

char* Page_html::create_error(char* code, char* text) // создаст ошибку с № и описанием. Сформирует ее в виде html, вернет html код в char*.
{
    _html = str_join(_html, (char *)"<p></p>\n<div>\n<table border=\"1\" bgcolor=\"#ff7f93\" align=\"center\" rules=\"rows\">\n<tbody>\n<tr>\n<td>\n<table border=\"60\" bgcolor=\"#ff7f93\" cellpadding=\"10\">\n<tr>\n<th>\n<h2>WEBSERV</h2>\n<h4>project</h4>\n<tr>\n<td rowspan=\"2\">\n<h3>");
    _html = str_join(_html, code);
    _html = str_join(_html, (char *)"</h3>\n<p>");
    _html = str_join(_html, text);
    _html = str_join(_html, (char *)"</p>\n</td>\n<td bgcolor=\"#42ff82\">\n<h4>Creators:</h4>\n<p><a><span>atomatoe</span></a></p>\n<p><a> <span>welease</span></a></p>\n<p><a><span>qtamaril </span></a></p>\n</td>\n</div>\n");
    return(_html);
}