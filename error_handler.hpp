/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 13:01:02 by atomatoe          #+#    #+#             */
/*   Updated: 2021/02/05 16:33:40 by atomatoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_HANDLER_HPP
# define ERROR_HANDLER_HPP

#include "includes.hpp"

class Error_handler
{
    private:
        char* _html;
    public:
        Error_handler() { _html = NULL; }
        ~Error_handler() { };
        char* create_error(char* code, char* text) // создаст ошибку с № и описанием. Сформирует ее в виде html, вернет html код в char*.
        {
            _html = str_join(_html, "<p></p>\n<div>\n<table border=\"1\" bgcolor=\"#ff7f93\" align=\"center\" rules=\"rows\">\n<tbody>\n<tr>\n<td>\n<table border=\"60\" bgcolor=\"#ff7f93\" cellpadding=\"10\">\n<tr>\n<th>\n<h2>WEBSERV</h2>\n<h4>project</h4>\n<tr>\n<td rowspan=\"2\">\n<h3>");
            _html = str_join(_html, code);
            _html = str_join(_html, "</h3>\n<p>");
            _html = str_join(_html, text);
            _html = str_join(_html, "</p>\n</td>\n<td bgcolor=\"#42ff82\">\n<h4>Creators:</h4>\n<p><a><span>atomatoe</span></a></p>\n<p><a> <span>welease</span></a></p>\n<p><a><span>qtamaril </span></a></p>\n</td>\n</div>\n");
            return(_html);
        }
};

#endif