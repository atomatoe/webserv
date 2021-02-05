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
// stat - http://www.codenet.ru/progr/cpp/spru/stat.php
// open/close/read dir - http://www.c-cpp.ru/content/closedir-opendir-readdir-rewinddir

class Autoindex
{
    private:
        char* _directory;
        char* _html;
        DIR* _dir;
        dirent* _file;
    public:
        Autoindex(char *directory) { _directory = directory; }
        ~Autoindex() { };
        char* create_page()
        {
            _html = str_join(_html, "<p></p>\n<div>\n<table border=\"1\" align=\"center\" rules=\"rows\">\n<tbody>\n<tr>\n<td>\n<table border=\"60\" bgcolor=\"#c9ce29\" cellpadding=\"10\">\n<tbody>\n<tr>\n<th>\n<h2>WEBSERV</h2>\n<h4>project</h4>\n</th>\n</tr>\n<tr>\n<td rowspan=\"2\">\n<h3>Index of ");
            _html = str_join(_html, _directory);
            _html = str_join(_html, "</h3>\n");
            _html = str_join(_html, "<a href=\"../\">../</a><br>\n"); // ссылка, br - переход на новую строку
            _html = str_join(_html, "</td>\n<td bgcolor=\"#a5eda5\">\n<h4>Creators:</h4>\n<p><a><span>atomatoe</span></a></p>\n<p><a> <span>welease</span></a></p>\n<p><a><span>qtamaril </span></a></p>\n</td>\n</div>\n");
            _dir = opendir(_directory);
            _file = readdir(_dir);
            std::cout << _file->d_name << std::endl;
            _file = readdir(_dir);
            std::cout << _file->d_name << std::endl;
            _file = readdir(_dir);
            std::cout << _file->d_name << std::endl;
            _file = readdir(_dir);
            std::cout << _file->d_name << std::endl;
            _file = readdir(_dir);
            std::cout << _file->d_name << std::endl;
            _file = readdir(_dir);
            std::cout << _file->d_name << std::endl;
            _file = readdir(_dir);
            std::cout << _file->d_name << std::endl;
            _file = readdir(_dir);
            std::cout << _file->d_name << std::endl;
            _file = readdir(_dir);
            std::cout << _file->d_name << std::endl;
            _file = readdir(_dir);
            std::cout << _file->d_name << std::endl;
            _file = readdir(_dir);
            std::cout << _file->d_name << std::endl;
            closedir(_dir);
            // // buffer = str_join(buffer, directory);
            // chdir(directory); // переходим в папку root
        
            // char* buffer = getcwd(NULL, 0);
            // if (buffer == NULL)
            //     perror("getcwd error");
            // std::cout << buffer << std::endl;
            return(_html);
        }
};

#endif