/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoindex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 16:47:33 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/05 19:30:36 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOINDEX_HPP
# define AUTOINDEX_HPP

#include "includes.hpp"
// stat - https://www.opennet.ru/man.shtml?topic=stat&category=2&russian=0
// open/close/read dir - http://www.c-cpp.ru/content/closedir-opendir-readdir-rewinddir

class Autoindex
{
    private:
        char* _html;
        DIR* _dir;
        dirent* _file;
        struct stat buff;
        char* get_way(char *s1, char *s2)
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
    public:
        Autoindex() { _html = NULL, _dir = NULL, _file = NULL; ft_memset(&buff, '\0', sizeof(buff)); }
        ~Autoindex() { };
        char* create_page(char * _directory)
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
};

#endif