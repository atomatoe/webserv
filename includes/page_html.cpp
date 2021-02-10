/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   page_html.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 15:48:01 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/10 15:56:25 by atomatoe         ###   ########.fr       */
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
    _html = str_join(_html, "<p></p>\n<div>\n<table border=\"1\" align=\"center\" rules=\"rows\">\n<tbody>\n<tr>\n<td>\n<table border=\"60\" bgcolor=\"#c9ce29\" cellpadding=\"10\">\n<tbody>\n<tr>\n<th>\n<h2>WEBSERV</h2>\n<h4>project</h4>\n</th>\n</tr>\n<tr>\n<td rowspan=\"2\">\n<h3>Index of ");
    _html = str_join(_html, _directory);
    _html = str_join(_html, "</h3>\n");
    while(_file = readdir(_dir))
    {
        _html = str_join(_html, "<a href=\"");
        _html = str_join(_html, _file->d_name);
        _html = str_join(_html, "\">");
        _html = str_join(_html, _file->d_name);
        if (_file->d_type == 4)
			_html = str_join(_html, "/");
        stat(get_way(_directory, _file->d_name), &buff); // нужен полный путь до файла
        std::cout << "test 2 = " << buff.st_mtimespec.tv_sec << std::endl;
        _html = str_join(_html, "</a><br>");
    }
    _html = str_join(_html, "</td>\n<td bgcolor=\"#a5eda5\">\n<h4>Creators:</h4>\n<p><a><span>atomatoe</span></a></p>\n<p><a> <span>welease</span></a></p>\n<p><a><span>qtamaril </span></a></p>\n</td>\n</div>\n");
    closedir(_dir);
    return(_html);
}

char* Page_html::create_error(char* code, char* text) // создаст ошибку с № и описанием. Сформирует ее в виде html, вернет html код в char*.
{
    _html = str_join(_html, "<p></p>\n<div>\n<table border=\"1\" bgcolor=\"#ff7f93\" align=\"center\" rules=\"rows\">\n<tbody>\n<tr>\n<td>\n<table border=\"60\" bgcolor=\"#ff7f93\" cellpadding=\"10\">\n<tr>\n<th>\n<h2>WEBSERV</h2>\n<h4>project</h4>\n<tr>\n<td rowspan=\"2\">\n<h3>");
    _html = str_join(_html, code);
    _html = str_join(_html, "</h3>\n<p>");
    _html = str_join(_html, text);
    _html = str_join(_html, "</p>\n</td>\n<td bgcolor=\"#42ff82\">\n<h4>Creators:</h4>\n<p><a><span>atomatoe</span></a></p>\n<p><a> <span>welease</span></a></p>\n<p><a><span>qtamaril </span></a></p>\n</td>\n</div>\n");
    return(_html);
}