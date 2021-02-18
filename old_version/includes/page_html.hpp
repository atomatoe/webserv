/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   page_html.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 16:47:33 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/10 15:56:35 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOINDEX_HPP
# define AUTOINDEX_HPP

#include "includes.hpp"
// stat - https://www.opennet.ru/man.shtml?topic=stat&category=2&russian=0
// open/close/read dir - http://www.c-cpp.ru/content/closedir-opendir-readdir-rewinddir

class Page_html
{
    private:
        char* _html;
        DIR* _dir;
        dirent* _file;
        struct stat buff;
        char* get_way(char *s1, char *s2);
    public:
        Page_html();
        ~Page_html();
        char* create_autoindex(char * _directory);
        char* create_error(char* code, char* text);
};

#endif